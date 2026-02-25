#include "pumpController.h"

PumpControllerStatus_t PumpController_Init() {
    /* HAL_Init should be run before this is called */

    // Init ADC
    if (!Temp_ADC_Init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init LEDs
    if (!LEDs_Init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init I2C
    if (!MX_I2C1_Init()) return PUMP_CONTROLLER_INIT_FAIL;

    // Init UART
    if (!mx_uart_init()) return PUMP_CONTROLLER_INIT_FAIL;

    return PUMP_CONTROLLER_OK;
}
