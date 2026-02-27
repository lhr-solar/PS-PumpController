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
#include "tasks.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                                    SHARED                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define BAUD_RATE 115200
#define DELAY_TIME 10000

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Pin_t;

typedef enum PumpControllerStatus {
    PUMP_CONTROLLER_OK,
    PUMP_CONTROLLER_INIT_FAIL,
    TEMP_OK,
    TEMP_INIT_FAIL,
    TEMP_ADC_START_FAIL,
    TEMP_ADC_READ_FAIL,
    TEMP_QUEUE_FULL,
} PumpControllerStatus_t;

PumpControllerStatus_t PumpController_Init();

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FANS + FAN CHIP                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

/**
  * @brief I2C Initialization Function
  */
bool MX_I2C1_Init(void);

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
#define PERCENT_0   0
#define PERCENT_25  25
#define PERCENT_50  50
#define PERCENT_75  75
#define PERCENT_100 100

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              TEMP SENSORS                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#define TEMP_TABLE_SIZE 4096    // 12 bit ADC
#define ADC_TASK_PRIO tskIDLE_PRIORITY + 4

typedef struct {
    int16_t temp_data;   // signed, 32 bit
    uint16_t adc_val;   // unsigned, 12 bit
} TempMsg_t;

bool Temp_ADC_Init();

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);

PumpControllerStatus_t Temp_StartADC(bool clearQueue);

PumpControllerStatus_t Temp_GetReading(TempMsg_t *message, TickType_t ticksToWait);

int16_t ADCToTemp(uint16_t adc_val);

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
extern const GPIO_Pin_t led_configs[];

/**
  * @brief LED Initialization Function
  */
bool LEDs_Init(void);

/**
  * @brief Turns on LED for a given amount of time
  */
void LED_Blink(GPIO_Pin_t led_config);

/**
  * @brief Turns on LED indefinitely
  */
void LED_On(GPIO_Pin_t led_pin);