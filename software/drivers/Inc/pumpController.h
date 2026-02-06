#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "stm32xx_hal.h"

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              SHARED STRUCTS                                   */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
typedef enum State_e {OFF = 0, ON} State;

typedef struct {
    GPIO_TypeDef* port;
    uint32_t pin;
} GPIO_Pin_t;

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FANS + FAN CHIP                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Fan pins
#define FAN_SCL_PORT            GPIOB
#define FAN_SCL_PIN             GPIO_PIN_6

#define FAN_SDA_PORT            GPIOB
#define FAN_SDA_PIN             GPIO_PIN_7

#define FAN_SMBALERT_PORT       GPIOB
#define FAN_SMBALERT_PIN        GPIO_PIN_12

// Fan macros
// [insert macros here]

// Fan functions
void Init_Task(void* argument);
void EMC2305_Task_1(void* argument);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c);

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              PUMP CONTROL                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Pump pins
#define PUMP_PWM_PORT           GPIOB
#define PUMP_PWM_PIN            GPIO_PIN_15

#define PUMP_TACH_PORT          GPIOA
#define PUMP_TACH_PIN           GPIO_PIN_11

// Pump macros
// [insert macros here]

// Pump functions
// [insert functions here]

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              TEMP SENSORS                                     */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Temp pins
#define TEMP1_ADC_PORT          GPIOA
#define TEMP1_ADC_PIN           GPIO_PIN_4

#define TEMP2_ADC_PORT          GPIOA
#define TEMP2_ADC_PIN           GPIO_PIN_5

// Temp macros
// [insert macros here]

// Temp functions
// [insert functions here]

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              FLOWRATE SENSOR                                  */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// Flowrate pins
#define FLOW_TACH_PORT          GPIOB
#define FLOW_TACH_PIN           GPIO_PIN_3

// Flowrate macros
// [insert macros here]

// Flowrate functions
// [insert functions here]

/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*                              STATUS LEDS                                      */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// LED pins
#define PUMP_LED_PORT           GPIOA
#define PUMP_LED_PIN            GPIO_PIN_0

#define FAN_LED_PORT            GPIOA
#define FAN_LED_PIN             GPIO_PIN_6

#define FANCHIP_LED_PORT        GPIOB
#define FANCHIP_LED_PIN         GPIO_PIN_14

#define FLOW_LED_PORT           GPIOB
#define FLOW_LED_PIN            GPIO_PIN_10

#define TEMP_LED_PORT           GPIOA
#define TEMP_LED_PIN            GPIO_PIN_12

#define STATUS_LED_PORT         GPIOB
#define STATUS_LED_PIN          GPIO_PIN_0

// LED macros
#define TOGGLE_TIME 500                                         // same as blinky

// LED structs
GPIO_Pin_t led_configs[] = {
    {PUMP_LED_PORT, PUMP_LED_PIN},
    {FAN_LED_PORT, FAN_LED_PIN},
    {FANCHIP_LED_PORT, FANCHIP_LED_PIN},
    {FLOW_LED_PORT, FLOW_LED_PIN},
    {TEMP_LED_PORT, TEMP_LED_PIN},
    {STATUS_LED_PORT, STATUS_LED_PIN}
};

// LED functions
void LEDs_Init(void);
void LED_Blink(GPIO_Pin_t led_pin);

#endif