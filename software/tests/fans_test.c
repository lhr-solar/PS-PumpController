#include "stm32xx_hal.h"
#include "pumpController.h"
#include "EMC2305.h"

int main(void) {
    // Init your HAL, System Clock, and Peripherals here
    HAL_Init();

    // Create tasks
    xTaskCreateStatic(Init_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);

    xTaskCreateStatic(EMC2305_Task_1,
        "EMC2305 Task 1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        emc2305TaskStack_1,
        &emc2305TaskBuffer_1);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}