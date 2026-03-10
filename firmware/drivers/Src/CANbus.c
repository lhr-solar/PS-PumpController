#include "CANbus.h"
#include "printf.h"

QueueHandle_t can_tx_queue;
uint8_t can_tx_qStorage[CAN_TX_QUEUE_LENGTH * CAN_TX_ITEM_SIZE];
static StaticQueue_t xStaticQueue_can_tx;

CarCAN_Status_t CAN_Init(void) {
    // taken from cubemx
  /* USER CODE BEGIN CAN1_Init 0 */
  /* Initialize queue */
    can_tx_queue = xQueueCreateStatic(
        CAN_TX_QUEUE_LENGTH, 
        CAN_TX_ITEM_SIZE, 
        can_tx_qStorage, 
        &xStaticQueue_can_tx
    );
    if (can_tx_queue == NULL) return CAN_INIT_FAIL;

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

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

  /* USER CODE END CAN1_Init 1 */
  hcan1->Instance = CAN1;
  hcan1->Init.Prescaler = 20;
  hcan1->Init.Mode = CAN_MODE_NORMAL;
  hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1->Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = DISABLE; // switched from disable
  hcan1->Init.ReceiveFifoLocked = DISABLE;
  hcan1->Init.TransmitFifoPriority = ENABLE;
  if (can_init(hcan1, &sFilterConfig) != CAN_OK)
  {
    return CAN_INIT_FAIL;
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  if (can_start(hcan1) != CAN_OK) {
    return CAN_INIT_FAIL;
  }

  /* USER CODE END CAN1_Init 2 */
    return CAN_INIT_OK;
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcan->Instance==CAN1) {
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  if(hcan->Instance==CAN1) {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }
}

// CarCAN_Status_t SendFlowrateCAN(FlowMsg_t *data, TickType_t ticksToWait) {
    


// }


// need     CAN_TxHeaderTypeDef header = {0}; in task

// static void PackFlowrateCANHeader(void) {
//     header->StdId = CAN_ID_PUMP_STATUS_FLOWRATE;
//     header->RTR = CAN_RTR_DATA;
//     header->IDE = CAN_ID_STD;
//     header->DLC = PUMP_STATUS_FLOWRATE_DLC;
//     header->TransmitGlobalTime = DISABLE;
//     return;
// }

static void PackTempCANHeader(CAN_TxHeaderTypeDef* header) {
    header->StdId = CAN_ID_COOLANT_TEMPERATURE;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = COOLANT_TEMPERATURE_DLC;
    header->TransmitGlobalTime = DISABLE;
    return;
}

// static void PackFanCANHeader(CAN_TxHeaderTypeDef* header) {
//     header->StdId = CAN_ID_RADIATOR_FANSPEED;
//     header->RTR = CAN_RTR_DATA;
//     header->IDE = CAN_ID_STD;
//     header->DLC = RADIATOR_FANSPEED_DLC;
//     header->TransmitGlobalTime = DISABLE;
//     return;
// }

// void PackFlowrateCANMessage(CAN_TxHeaderTypeDef* header, FlowMsg_t* FlowMsg) {

// }

// set struct in task
// [CHANGE THIS] - dbc currently has 16 bits, both temp sensors
void PackTempCANMessage(CAN_TxHeaderTypeDef* header, TempMsg_t* message, uint8_t tx_data[8]) {
    PackTempCANHeader(header);
    // memcpy(&tx_data[4], &(message->temp_data), sizeof(int32_t));
    // memcpy(&tx_data[0], &(message->temp_data), sizeof(int32_t));
    
    
    tx_data[0] = (message->temp_data & 0x00C0) >> 6;
    tx_data[1] = (message->temp_data & 0x0030) >> 4;
    tx_data[2] = (message->temp_data & 0x000C) >> 2;
    tx_data[3] = message->temp_data & 0x0003;
    // printf("data to send: %d \n\r", tx_data[0]);
}

// void PackFanCANMessage(CAN_TxHeaderTypeDef* header, FlowMsg_t* FlowMsg) {

// }


// #define CAN_ID_PUMP_STATUS_FLOWRATE 0x500
// #define CAN_ID_COOLANT_TEMPERATURE 0x501
// #define CAN_ID_RADIATOR_FANSPEED 0x502


// typedef struct {
//     uint8_t Pump_DutyCycle;
//     uint8_t Pump_Fault;
//     uint16_t FlowRate_1;
//     uint16_t FlowRate_2;
// } pump_status_flowrate_t; // in L/min

// typedef struct {
//     int16_t Coolant_Temperature_1;
//     int16_t Coolant_Temperature_2;
// } coolant_temperature_t; // in celsius

// typedef struct {
//     uint16_t Radiator_Fan_Speed_Measurement_1;
//     uint16_t Radiator_Fan_Speed_Measurement_2;
//     uint16_t Radiator_Fan_Speed_Target_1;
//     uint16_t Radiator_Fan_Speed_Target_2;
// } radiator_fanspeed_t; // in RPM