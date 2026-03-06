#include "pumpController.h"

PumpController_Status_t PumpController_Init() {
    /* HAL_Init should be run before this is called */

    // Init LEDs
    if (!LEDs_Init()) Error_Handler();

    // Init UART printf
    if (!mx_uart_init()) Error_Handler();

    // Init TIM2 flowrate
    if (!MX_TIM2_Init()) Error_Handler();
    
    // Init ADC temp sensor
    if (!Temp_ADC_Init()) Error_Handler();

    // Init I2C fanchip
    if (!MX_I2C1_Init()) Error_Handler();
    

    return PUMP_CONTROLLER_OK;
}
