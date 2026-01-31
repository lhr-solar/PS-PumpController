#include"<stm32xx_hal.h"
#include "common.h"

// Fan pins
#define FAN_SCL_PORT            GPIOB
#define FAN_SCL_PIN             GPIO_PIN_6

#define FAN_SDA_PORT            GPIOB
#define FAN_SDA_PIN             GPIO_PIN_7

#define FAN_SMBALERT_PORT       GPIOB
#define FAN_SMBALERT_PIN        GPIO_PIN_12

// Pump pins
#define PUMP_PWM_PORT           GPIOB
#define PUMP_PWM_PIN            GPIO_PIN_15

#define PUMP_TACH_PORT          GPIOA
#define PUMP_TACH_PIN           GPIO_PIN_11

// Temp pins
#define TEMP1_ADC_PORT          GPIOA
#define TEMP1_ADC_PIN           GPIO_PIN_4

#define TEMP2_ADC_PORT          GPIOA
#define TEMP2_ADC_PIN           GPIO_PIN_5

// Flowrate pins
#define FLOW_TACH_PORT          GPIOB
#define FLOW_TACH_PIN           GPIO_PIN_3

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