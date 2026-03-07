#include "PumpController.h"

PumpController_Status_t PumpController_Init() {
    /* HAL_Init should be run before this is called */

    // Init LEDs
    if (!LEDs_Init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init UART printf
    if (!mx_uart_init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init TIM2 flowrate
    if (!MX_TIM2_Init()) return PUMP_CONTROLLER_INIT_FAIL;
    
    // Init ADC temp sensor
    if (!Temp_ADC_Init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init I2C fanchip
    if (!MX_I2C1_Init()) return PUMP_CONTROLLER_INIT_FAIL;
    

    return PUMP_CONTROLLER_OK;
}
