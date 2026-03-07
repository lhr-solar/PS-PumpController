#pragma once

#include "pindefs.h"
#include "tempTable.h"

#include "stm32xx_hal.h"
#include "ADC.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              TEMP SENSORS                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define TEMP_TABLE_SIZE     4096    // 12 bit ADC

// returned by Temp_ADC_Init() and Temp_StartADC()
typedef enum Temp_Status {
    TEMP_INIT_FAIL,
    TEMP_OK,
    TEMP_ADC_START_FAIL,
    TEMP_ADC_READ_FAIL,
    TEMP_QUEUE_FULL,
} Temp_Status_t;

// stores temperature data and raw ADC value for each reading
typedef struct {
    int16_t temp_data;
    uint16_t adc_val;
} TempMsg_t;

/**
* @brief Initializes the ADC queue and struct for the temperature sensor
* @return Temp_Status_t indicating failure (0) or success (1) of initialization
*/
Temp_Status_t Temp_ADC_Init();

/**
* @brief Initializes the ADC MSP (GPIO and clock) for the temperature sensor
* @param hadc: pointer to ADC handle structure
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);

/**
* @brief Initializes the ADC queue and struct for the temperature sensor
* @param clearQueue: if true, clears the ADC queue before starting the ADC conversion
* @return Temp_Status_t indicating failure (0) or success (1) of initialization
*/
Temp_Status_t Temp_StartADC(bool clearQueue);

/**
* @brief Converts value in ADC queue to temperature using lookup table and stores in message struct
* @param message: pointer to TempMsg_t struct to store ADC value and converted temperature data
* @return Temp_Status_t indicating failure (0) or success (1) of reading from queue and converting to temperature
*/
Temp_Status_t Temp_GetReading(TempMsg_t *message, TickType_t ticksToWait);

/**
* @brief Converts raw ADC value to temperature using lookup table
* @param adc_val: raw ADC value to be converted to temperature
* @return int16_t temperature corresponding to the given ADC value
*/
int16_t ADCToTemp(uint16_t adc_val);