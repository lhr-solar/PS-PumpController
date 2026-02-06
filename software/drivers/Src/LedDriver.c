#include "pumpController.h"
#include "stm32xx_hal.h"

// initialize an individual LED
void LED_Init(GPIO_Pin_t led_config) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = led_config.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(led_config.port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(led_config.port, led_config.pin, GPIO_PIN_RESET);
}

// initialize all LED GPIOs
void LEDs_Init(void) {    
    for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
        LED_Init(led_configs[i]);
    }
}

void LED_Blink(GPIO_Pin_t led_config) {
    HAL_GPIO_TogglePin(led_config.port, led_config.pin);
    HAL_Delay(TOGGLE_TIME);
    HAL_GPIO_TogglePin(led_config.port, led_config.pin);
    HAL_Delay(TOGGLE_TIME);
}
