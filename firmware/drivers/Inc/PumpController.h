#pragma once

#include "pindefs.h"
#include "common.h"

#include "stm32xx_hal.h"
#include "printf.h"
#include "UART.h"
#include "Leds.h"
#include "Fan_I2C.h"
#include "Flowrate.h"
#include "Temp.h"
#include "CANbus.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                                    SHARED                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define UART_BAUD_RATE 115200

// stores gpio pins concisely
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Pin_t;

// returned by PumpController_Init()
typedef enum PumpController_Status {
    PUMP_CONTROLLER_INIT_FAIL,
    PUMP_CONTROLLER_OK
} PumpController_Status_t;

/**
* @brief Initializes all peripherals (LEDs, UART, ADC, I2C) used by the pump controller. Should be called before any tasks are created.
* @return PumpController_Status_t indicating failure (0) or success (1) of initializations
*/
PumpController_Status_t PumpController_Init();