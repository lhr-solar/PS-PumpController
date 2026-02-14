#include "pumpController.h"
#include "stm32xx_hal.h"

GPIO_Pin_t led_configs[] = {
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

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = led_config.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(led_config.port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(led_config.port, led_config.pin, GPIO_PIN_RESET);
}

// initialize all LED GPIOs
void LEDs_Init(void) {    
    // for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
    //     LED_Init(led_configs[i]);
    // }
    LED_Init(led_configs[PUMP_LED]);
    LED_Init(led_configs[FAN_LED]);
    LED_Init(led_configs[FANCHIP_LED]);
    LED_Init(led_configs[FLOW_LED]);
    LED_Init(led_configs[TEMP_LED]);
    LED_Init(led_configs[STATUS_LED]);

    // PSOM status LED init
    GPIO_InitTypeDef led_init = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = STATUS_LED_PIN_1 | STATUS_LED_PIN_2 | STATUS_LED_PIN_3,
    };
    HAL_GPIO_Init(STATUS_LED_PORT, &led_init);

    // PSOM heartbeat LED init
    // GPIO_InitTypeDef hb_init = {
    //     .Mode = GPIO_MODE_OUTPUT_PP,
    //     .Pull = GPIO_NOPULL,
    //     .Pin = GPIO_PIN_11,
    // };
}

void LED_Blink(GPIO_Pin_t led_config) {
    HAL_GPIO_TogglePin(led_config.port, led_config.pin);
    HAL_Delay(TOGGLE_TIME);
    HAL_GPIO_TogglePin(led_config.port, led_config.pin);
    HAL_Delay(TOGGLE_TIME);
}

void LED_On(GPIO_Pin_t led_config) {
    HAL_GPIO_WritePin(led_config.port, led_config.pin, GPIO_PIN_SET);
}
