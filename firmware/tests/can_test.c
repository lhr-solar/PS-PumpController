/* CAN test

- Setups up CANs in loopback mode
- Send 4 messages (since there are 3 mailboxes, 1 ends up going in the can1 send queue)
- receives the 4 messages and verifies correctness
- Flashes LED if successful on all CANs

- Included ClockConfig / CAN_MspInit is for PSOM

*/

/*
#define CAN_ID_PUMP_STATUS_FLOWRATE 0x500
#define CAN_ID_COOLANT_TEMPERATURE 0x501
#define CAN_ID_RADIATOR_FANSPEED 0x502
*/

#include "stm32xx_hal.h"
#include "CANbus.h"
#include "CarCAN_can_msgs.h"

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];
// CAN_HandleTypeDef* hcan1;

static void success_handler(void) {
  GPIO_InitTypeDef led_init = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = GPIO_PIN_0
  };
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &led_init);

  while(1){
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    HAL_Delay(500);
  }
}

static void task(void *pvParameters) {
  // create payload to send
  CAN_TxHeaderTypeDef tx_header = {0};   
  tx_header.StdId = CAN_ID_PUMP_STATUS;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.IDE = CAN_ID_STD;
  tx_header.DLC = 2;
  tx_header.TransmitGlobalTime = DISABLE;

  // --- TEST ID: PUMP_STATUS_FLOWRATE (0x500) ---
  uint8_t tx_data[8] = {0};
  tx_data[0] = 0x01;
  tx_data[1] = 0x00;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();

  tx_data[0] = 0x02;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();

  // --- TEST ID: COOLANT_TEMPERATURE (0x501) ---
  tx_data[0] = 0x03;
  tx_header.StdId = CAN_ID_COOLANT_TEMPERATURE;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();

  tx_data[0] = 0x04;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();

  // Receive and verify PUMP_STATUS_FLOWRATE
  CAN_RxHeaderTypeDef rx_header = {0};
  uint8_t rx_data[8] = {0};
  can_status_t status;

  status = can_recv(hcan1, CAN_ID_PUMP_STATUS_FLOWRATE, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK || rx_data[0] != 0x01) Error_Handler();
  
  status = can_recv(hcan1, CAN_ID_PUMP_STATUS_FLOWRATE, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK || rx_data[0] != 0x02) Error_Handler();

  // Ensure no more messages are in the 0x500 queue (non-blocking)
  status = can_recv(hcan1, CAN_ID_PUMP_STATUS_FLOWRATE, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();

  // Receive and verify COOLANT_TEMPERATURE
  status = can_recv(hcan1, CAN_ID_COOLANT_TEMPERATURE, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK || rx_data[0] != 0x03) Error_Handler();

  status = can_recv(hcan1, CAN_ID_COOLANT_TEMPERATURE, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK || rx_data[0] != 0x04) Error_Handler();

  // --- TEST QUEUE OVERWRITE: RADIATOR_FANSPEED (0x502) ---
  // This ID is marked 'true' (circular) in recv_entries.h
  tx_header.StdId = CAN_ID_RADIATOR_FANSPEED;
  
  // Fill the queue and then some (Queue size is 5)
  // We send 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
  // Because it is circular, 0x05 should be dropped when 0x0A arrives
  for (uint8_t i = 0x05; i <= 0x0A; i++) {
    tx_data[0] = i;
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  }

  // Brief delay to allow ISR to process all loopback messages
  vTaskDelay(pdMS_TO_TICKS(100));

  // Receive the remaining 5 messages (0x06 through 0x0A)
  for (uint8_t i = 0x06; i <= 0x0A; i++) {
    status = can_recv(hcan1, CAN_ID_RADIATOR_FANSPEED, &rx_header, rx_data, portMAX_DELAY);
    if (status != CAN_OK || rx_data[0] != i) Error_Handler();
  }

  // Queue should now be empty
  status = can_recv(hcan1, CAN_ID_RADIATOR_FANSPEED, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();

  success_handler();
}

int main(void) {
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) Error_Handler();
  SystemClock_Config();
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);

  // create filter
  CAN_FilterTypeDef  sFilterConfig;
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  // setup can1 init
  // Baud rate is 250 kbit/s
  hcan1->Init.Prescaler = 20;
  hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1->Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1->Init.Mode = CAN_MODE_LOOPBACK;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = ENABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;

  // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
  // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
  hcan1->Init.TransmitFifoPriority = ENABLE;


  // initialize CAN1
  if (can_init(hcan1, &sFilterConfig) != CAN_OK) Error_Handler();

  if (can_start(hcan1) != CAN_OK) Error_Handler();

  xTaskCreateStatic(
                task,
                "task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                task_stack,
                &task_buffer);

  vTaskStartScheduler();

  Error_Handler();

  return 0;
}