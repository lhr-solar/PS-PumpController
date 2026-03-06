#pragma once

#include "pindefs.h"
#include "printf.h"
#include "stm32xx_hal.h"
#include "EMC2305.h"
#include "common.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FANS + FAN CHIP                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/


// extern EMC2305_HandleTypeDef chip;

// returned by MX_I2C1_Init()
typedef enum FanChip_Status {
    FAN_CHIP_INIT_FAIL,
    FAN_CHIP_OK,
    FAN_CHIP_WRITE_FAIL,
    FAN_CHIP_READ_FAIL,
} FanChip_Status_t;

typedef enum FanChip_Device {
    FAN1,
    FAN2,
    PUMP,
} FanChip_Device_t;

typedef enum FanChip_Mode {
    PWM_MODE,
    FSC_MODE,
} FanChip_Mode_t;

/**
* @brief I2C Initialization Function
* @return FanChip_Status_t indicating failure (0) or success (1) of initialization
*/
FanChip_Status_t MX_I2C1_Init(void);

/**
* @brief I2C Transmit Interrupt Callback
* @param hi2c: pointer to I2C handle structure
*/
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c);

/**
* @brief I2C Recieve Interrupt Callback
* @param hi2c: pointer to I2C handle structure
*/
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c);

/**
* @brief This function handles I2C1 event interrupt.
*/
void I2C1_EV_IRQHandler(void);

/**
* @brief This function handles I2C1 error interrupt.
*/
void I2C1_ER_IRQHandler(void);

/**
* @brief Initializes the fan chip (EMC2305) over I2C, called at the start of fanchip task
* @return FanChip_Status_t indicating failure (0) or success (1) of initialization
*/
FanChip_Status_t FanChip_Init(FanChip_Device_t device, FanChip_Mode_t mode);


/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              PUMP CONTROL                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Pump macros
#define PERCENT_0           0
#define PERCENT_25          25
#define PERCENT_50          50
#define PERCENT_75          75
#define PERCENT_100         100