#pragma once

#include "pindefs.h"
#include "stm32xx_hal.h"
#include "common.h"
#include "CAN.h"
#include "Flowrate.h"
#include "Fan_I2C.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                                 CAR CAN BUS                                   */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define I2C_INTERRUPT_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

#define PUMP_STATUS_FLOWRATE_QUEUE_SIZE     5
#define COOLANT_TEMPERATURE_QUEUE_SIZE      5
#define RADIATOR_FANSPEED_QUEUE_SIZE        5

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