#pragma once

#include "pumpController.h"
#include "pindefs.h"

#include "stm32xx_hal.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FLOWRATE SENSOR                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Flowrate macros + structs


typedef enum Flow_Status {
    FLOWRATE_FAIL,
    FLOWRATE_OK
} Flow_Status_t;

typedef struct {
    uint32_t diff;
    uint16_t freq;
    uint16_t flowrate_x10;
} FlowMsg_t;

// Flowrate functions
// [insert functions here]
Flow_Status_t MX_TIM2_Init(void);
void TIM2_IRQHandler(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);