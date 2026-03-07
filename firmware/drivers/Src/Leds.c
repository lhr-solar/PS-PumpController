#include "Leds.h"

static const GPIO_Pin_t led_configs[] = {
    {PUMP_LED_PORT, PUMP_LED_PIN},
    {FAN_LED_PORT, FAN_LED_PIN},
    {FANCHIP_LED_PORT, FANCHIP_LED_PIN},
    {FLOW_LED_PORT, FLOW_LED_PIN},
    {TEMP_LED_PORT, TEMP_LED_PIN},
    {PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN}
};

// initialize an individual LED
void LED_Init(GPIO_Pin_t led_config) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = led_config.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(led_config.port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(led_config.port, led_config.pin, GPIO_PIN_RESET);

    return;
}

// initialize all LED GPIOs
LED_Status_t LEDs_Init(void) {    
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    for(int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
        LED_Init(led_configs[i]);
    }

    return LED_OK;
}