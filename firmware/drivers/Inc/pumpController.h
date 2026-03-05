#pragma once

#include "stm32xx_hal.h"
#include "printf.h"
#include "FreeRTOS.h"
#include "UART.h"

#include "pindefs.h"
#include "common.h"
#include "tasks.h"
#include "Fan_I2C.h"
#include "FlowrateDriver.h"
#include "LedDriver.h"
#include "TempDriver.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                                    SHARED                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define UART_BAUD_RATE 115200
#define DELAY_TIME 10000

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Pin_t;

typedef enum PumpController_Status {
    PUMP_CONTROLLER_INIT_FAIL,
    PUMP_CONTROLLER_OK
} PumpController_Status_t;

PumpController_Status_t PumpController_Init();