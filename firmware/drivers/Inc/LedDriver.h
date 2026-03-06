#pragma once

#include "pumpController.h"
#include "pindefs.h"

#include "stm32xx_hal.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              STATUS LEDS                                      */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define TOGGLE_TIME 500     // same as blinky

// returned by LEDs_Init()
typedef enum LED_Status {
    LED_INIT_FAIL,
    LED_OK
} LED_Status_t;

/**
* @brief LED Initialization Function
* @return LED_Status_t indicating failure (0) or success (1) of initialization
*/
LED_Status_t LEDs_Init(void);