#include "pumpController.h"

// initialize all LED GPIOs
void LED_Init(void) {
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = PUMP_LED_PIN
    };
    gpio_clock_enable((uint32_t)PUMP_LED_PIN);

    HAL_GPIO_Init(PUMP_LED_PORT, &led_config);

    led_config.Pin = FAN_LED_PIN;
    HAL_GPIO_Init(FAN_LED_PORT, &led_config);

    led_config.Pin = FANCHIP_LED_PIN;
    HAL_GPIO_Init(FANCHIP_LED_PORT, &led_config);

    led_config.Pin = FLOW_LED_PIN;
    HAL_GPIO_Init(FLOW_LED_PORT, &led_config);

    led_config.Pin = TEMP_LED_PIN;
    HAL_GPIO_Init(TEMP_LED_PORT, &led_config);

    led_config.Pin = STATUS_LED_PIN;
    HAL_GPIO_Init(STATUS_LED_PORT, &led_config);
}

void LED_Toggle(GPIO_Pin_t led_pin) {
    HAL_GPIO_TogglePin(led_pin.port, led_pin.pin);
    HAL_Delay(TOGGLE_TIME);
}
