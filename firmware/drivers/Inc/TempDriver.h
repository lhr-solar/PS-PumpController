#pragma once

#include "pumpController.h"
#include "pindefs.h"
#include "tempTable.h"

#include "stm32xx_hal.h"
#include "ADC.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              TEMP SENSORS                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define TEMP_TABLE_SIZE     4096    // 12 bit ADC
#define ADC_TASK_PRIO       tskIDLE_PRIORITY + 4

typedef enum Temp_Status {
    TEMP_INIT_FAIL,
    TEMP_OK,
    TEMP_ADC_START_FAIL,
    TEMP_ADC_READ_FAIL,
    TEMP_QUEUE_FULL,
} Temp_Status_t;

typedef struct {
    int16_t temp_data;   // signed, 32 bit
    uint16_t adc_val;   // unsigned, 12 bit
} TempMsg_t;

/**
* @brief Initializes the ADC for the temperature sensor and creates the ADC task
*/
Temp_Status_t Temp_ADC_Init();

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);

Temp_Status_t Temp_StartADC(bool clearQueue);

Temp_Status_t Temp_GetReading(TempMsg_t *message, TickType_t ticksToWait);

int16_t ADCToTemp(uint16_t adc_val);