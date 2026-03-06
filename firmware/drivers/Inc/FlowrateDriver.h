#pragma once

#include "pindefs.h"

#include "stm32xx_hal.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FLOWRATE SENSOR                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// returned by MX_TIM2_Init()
typedef enum Flow_Status {
    FLOWRATE_FAIL,
    FLOWRATE_OK
} Flow_Status_t;

// stores flowrate calculations for each rising edge
typedef struct {
    uint32_t diff;
    uint16_t freq;
    uint16_t flowrate_x10;
} FlowMsg_t;

/**
  * @brief TIM2 Initialization Function
  * @return Flow_Status_t indicating failure (0) or success (1) of initialization
  */
Flow_Status_t MX_TIM2_Init(void);

/**
  * @brief TIM2 Interrupt Handler
  */
void TIM2_IRQHandler(void);

/**
  * @brief TIM2 Input Capture Callback, called on rising edge to calculate time diff
  * @param htim: pointer to TIM handle structure
  * @retval None, sends time diff to flowrate queue
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);