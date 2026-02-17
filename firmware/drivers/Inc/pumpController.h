#pragma once

#include "stm32xx_hal.h"
#include "pindefs.h"
#include "EMC2305.h"
#include "printf.h"
#include "FreeRTOS.h"
#include "UART.h"
#include "common.h"
#include "tempTable.h"
#include "ADC.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              SHARED STRUCTS                                   */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
typedef enum State_e {OFF = 0, ON} State_t;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Pin_t;

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FANS + FAN CHIP                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

/**
  * @brief I2C Initialization Function
  */
void MX_I2C1_Init(void);

/**
  * @brief I2C Transmit Interrupt Callback
  */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c);

/**
  * @brief I2C Recieve Interrupt Callback
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

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              PUMP CONTROL                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Pump macros
// [insert macros here]

// Pump functions
// [insert functions here]

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              TEMP SENSORS                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Temp macros
// [insert macros here]
extern ADC_HandleTypeDef hADC1;


/**
  * @brief This function converts ADC reading to a temperature
  * @param adc_value: the raw ADC value read from the temp sensor
  * @return the corresponding temperature in degrees Celsius
  */
int getTemp(void);

/**
  * @brief This function prints the current temperature
  * @param adc_value: the raw ADC value read from the temp sensor
  */
void printTemp(void);

/**
  * @brief ADC Initialization Function
  */
void MX_ADC1_Init(void);

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FLOWRATE SENSOR                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Flowrate macros
// [insert macros here]

// Flowrate functions
// [insert functions here]

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              STATUS LEDS                                      */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define TOGGLE_TIME 500     // same as blinky
enum LED_Index {PUMP_LED = 0, FAN_LED, FANCHIP_LED, FLOW_LED, TEMP_LED, STATUS_LED};

extern GPIO_Pin_t led_configs[6];

/**
  * @brief LED Initialization Function
  */
void LEDs_Init(void);

/**
  * @brief Turns on LED for a given amount of time
  */
void LED_Blink(GPIO_Pin_t led_pin);

/**
  * @brief Turns on LED indefinitely
  */
void LED_On(GPIO_Pin_t led_pin);