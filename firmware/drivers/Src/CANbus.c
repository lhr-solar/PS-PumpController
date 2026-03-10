#include "CANbus.h"

extern CAN_HandleTypeDef* hcan1;

CarCAN_Status_t CAN_Init(void) {
    // taken from cubemx
  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

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
  hcan1->Init.AutoRetransmission = DISABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;
  hcan1->Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(hcan1) != HAL_OK)
  {
    return CAN_INIT_FAIL;
  }
  /* USER CODE BEGIN CAN1_Init 2 */

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
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
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

CarCAN_Status_t SendFlowrateCAN(FlowMsg_t *data, TickType_t ticksToWait) {
    


}


// need     CAN_TxHeaderTypeDef header = {0}; in task

static void PackFlowrateCANHeader(void) {
    header->StdId = CAN_ID_PUMP_STATUS_FLOWRATE;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = PUMP_STATUS_FLOWRATE_DLC;
    header->TransmitGlobalTime = DISABLE;
    return;
}

static void PackTempCANHeader(CAN_TxHeaderTypeDef* header) {
    header->StdId = CAN_ID_COOLANT_TEMPERATURE;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = COOLANT_TEMPERATURE_DLC;
    header->TransmitGlobalTime = DISABLE;
    return;
}

static void PackFanCANHeader(CAN_TxHeaderTypeDef* header) {
    header->StdId = CAN_ID_RADIATOR_FANSPEED;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = RADIATOR_FANSPEED_DLC;
    header->TransmitGlobalTime = DISABLE;
    return;
}

void PackFlowrateCANMessage(CAN_TxHeaderTypeDef* header, FlowMsg_t* FlowMsg) {

}


#define CAN_ID_PUMP_STATUS_FLOWRATE 0x500
#define CAN_ID_COOLANT_TEMPERATURE 0x501
#define CAN_ID_RADIATOR_FANSPEED 0x502

#define PUMP_STATUS_FLOWRATE_DLC        sizeof(pump_status_flowrate_t)
#define COOLANT_TEMPERATURE_DLC         sizeof(coolant_temperature_t)
#define RADIATOR_FANSPEED_DLC           sizeof(radiator_fanspeed_t)


typedef struct {
    uint8_t Pump_DutyCycle;
    uint8_t Pump_Fault;
    uint16_t FlowRate_1;
    uint16_t FlowRate_2;
} pump_status_flowrate_t; // in L/min

typedef struct {
    int16_t Coolant_Temperature_1;
    int16_t Coolant_Temperature_2;
} coolant_temperature_t; // in celsius

typedef struct {
    uint16_t Radiator_Fan_Speed_Measurement_1;
    uint16_t Radiator_Fan_Speed_Measurement_2;
    uint16_t Radiator_Fan_Speed_Target_1;
    uint16_t Radiator_Fan_Speed_Target_2;
} radiator_fanspeed_t; // in RPM