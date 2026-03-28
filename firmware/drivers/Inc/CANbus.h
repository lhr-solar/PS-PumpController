#pragma once

#include "pindefs.h"
#include "stm32xx_hal.h"
#include "common.h"
#include "CAN.h"
#include "Flowrate.h"
#include "Fan_I2C.h"
#include "Temp.h"
#include "CarCAN_can_msgs.h"
#include <string.h>

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                                 CAR CAN BUS                                   */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define CAN_INTERRUPT_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

#define PUMP_STATUS_FLOWRATE_QUEUE_SIZE     5
#define COOLANT_TEMPERATURE_QUEUE_SIZE      5
#define RADIATOR_FANSPEED_QUEUE_SIZE        5

#define PUMP_STATUS_FLOWRATE_DLC        sizeof(pump_status_flowrate_t)
#define COOLANT_TEMPERATURE_DLC         sizeof(coolant_temperature_t)
#define RADIATOR_FANSPEED_DLC           sizeof(radiator_fanspeed_t)

#define CAN_TX_ITEM_SIZE sizeof(can_tx_payload_t)
#define CAN_TX_QUEUE_LENGTH 20
extern QueueHandle_t can_tx_queue;

/*
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_0 0x2
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_1 0x3
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_2 0x4
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_3 0x5
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_4 0x6
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_5 0x7
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_6 0x8
#define CAN_ID_BPS_VOLTAGE_TEMPERATURE_7 0x9
*/

// returned by CAN_Init()
typedef enum CarCAN_Status {
    CAN_INIT_FAIL,
    CAN_INIT_OK,
    CAN_SEND_FAIL,
    CAN_SEND_OK,
} CarCAN_Status_t;

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
CarCAN_Status_t CAN_Init(void);

/**
  * @brief CAN MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);

/**
  * @brief CAN MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan);

void PackTempCANMessage(CAN_TxHeaderTypeDef* header, TempMsg_t* message, uint8_t tx_data[8]);

void PackFlowrateCANMessage(CAN_TxHeaderTypeDef* header, pump_status_flowrate_t* FlowMsg, uint8_t tx_data[8]);

void PackFanCANMessage(CAN_TxHeaderTypeDef* header, radiator_fanspeed_t* FanMsg, uint8_t tx_data[8]);