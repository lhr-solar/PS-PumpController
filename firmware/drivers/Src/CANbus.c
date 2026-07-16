#include "CANbus.h"
#include "printf.h"

// QueueHandle_t can_tx_queue;
// uint8_t can_tx_qStorage[CAN_TX_QUEUE_LENGTH * CAN_TX_ITEM_SIZE];
// static StaticQueue_t xStaticQueue_can_tx;

uint32_t temp_counter = 0;
#define ALL_TAPS_RECIEVED 0xFFFF

CarCAN_Status_t CAN_Init(void) {

    // removed GPIO init block, it exists in MSP
    // Create Filter
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = (0x00B << 5); 
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = (0x7FF << 5); 
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    // Setup CAN1 Initialization
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
    
    // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
    // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
    hcan1->Init.TransmitFifoPriority = ENABLE;

    // Initialize CAN1
    if (can_init(hcan1, &sFilterConfig) != CAN_OK) { 
        return CAN_INIT_FAIL;
    }
    // Start CAN1
    if (can_start(hcan1) != CAN_OK) { 
        return CAN_INIT_FAIL;
    }
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

void PackFlowrateCANMessage(CAN_TxHeaderTypeDef* header, pump_status_t* FlowMsg, uint8_t tx_data[8]) {
    header->StdId = CAN_ID_PUMP_STATUS;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = PUMP_STATUS_FLOWRATE_DLC;
    header->TransmitGlobalTime = DISABLE;

    tx_data[0] = FlowMsg->Pump_DutyCycle;
    tx_data[1] = FlowMsg->Pump_DutyCycle;
    tx_data[2] = (FlowMsg->FlowRate_1 & 0xFF00) >> 8;
    tx_data[3] = (FlowMsg->FlowRate_1 & 0xFF);
    tx_data[4] = (FlowMsg->FlowRate_2 & 0xFF00) >> 8;
    tx_data[5] = (FlowMsg->FlowRate_2 & 0xFF);

    return;
}

void PackTempCANMessage(CAN_TxHeaderTypeDef* header, TempMsg_t* message, uint8_t tx_data[8]) {

    header->StdId = CAN_ID_COOLANT_TEMPERATURE;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = COOLANT_TEMPERATURE_DLC;
    header->TransmitGlobalTime = DISABLE;

    tx_data[0] = (message->temp_data & 0xFF000000) >> 24;
    tx_data[1] = (message->temp_data & 0x00FF0000) >> 16;
    tx_data[2] = (message->temp_data & 0x0000FF00) >> 8;
    tx_data[3] = (message->temp_data & 0x000000FF);
}

void PackFanCANMessage(CAN_TxHeaderTypeDef* header, radiator_fanspeed_t* FanMsg, uint8_t tx_data[8]) {
    header->StdId = CAN_ID_RADIATOR_FANSPEED;
    header->RTR = CAN_RTR_DATA;
    header->IDE = CAN_ID_STD;
    header->DLC = RADIATOR_FANSPEED_DLC;
    header->TransmitGlobalTime = DISABLE;

    tx_data[0] = (FanMsg->Radiator_Fan_Speed_Measurement_1 & 0xFF00) >> 8;
    tx_data[1] = (FanMsg->Radiator_Fan_Speed_Measurement_1 & 0xFF);
    tx_data[2] = (FanMsg->Radiator_Fan_Speed_Measurement_2 & 0xFF00) >> 8;
    tx_data[3] = (FanMsg->Radiator_Fan_Speed_Measurement_2 & 0xFF);
    tx_data[4] = (FanMsg->Radiator_Fan_Speed_Target_1 & 0xFF00) >> 8;
    tx_data[5] = (FanMsg->Radiator_Fan_Speed_Target_1 & 0xFF);
    tx_data[6] = (FanMsg->Radiator_Fan_Speed_Target_2 & 0xFF00) >> 8;
    tx_data[7] = (FanMsg->Radiator_Fan_Speed_Target_2 & 0xFF);

    return;
}

void UnpackBPSStatusMessage(bps_status_msg_t* msg, uint16_t id, const uint8_t rx_data[8]) {
    msg->BPS_Fault = rx_data[BPS_FAULT_INDEX];
    msg->Main_Battery_Avg_Temperature = ((rx_data[BPS_TEMP_HIGH_INDEX] << 8) | rx_data[BPS_TEMP_LOW_INDEX]);
    return;
}