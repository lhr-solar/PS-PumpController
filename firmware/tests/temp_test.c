// basic test to verify if thermistor reading is correctly converted

#include "pumpController.h"

int main(void) {
    HAL_Init();
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init peripherals
    mx_uart_init();
    MX_I2C1_Init();
    LEDs_Init();
    ADC_Init();

    while (1) {
        printTemp();
    }

    return 0;
}