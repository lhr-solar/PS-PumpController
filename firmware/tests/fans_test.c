#include "stm32xx_hal.h"
#include "pumpController.h"
#include "EMC2305.h"
#include "printf.h"

#include <stdio.h>

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_1;
StackType_t emc2305TaskStack_1[configMINIMAL_STACK_SIZE];
//static StaticTask_t emc2305TaskBuffer_2;
//static StackType_t emc2305TaskStack_2[configMINIMAL_STACK_SIZE];

int main(void) {
    // initialize the HAL and system clock
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init peripherals
    MX_I2C1_Init();

    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
    printf("Task 2: EMC2305 Initialized\r\n");

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

    // xTaskCreateStatic(EMC2305_Task_2,
    //     "EMC2305 Task 2",
    //     configMINIMAL_STACK_SIZE,
    //     NULL,
    //     tskIDLE_PRIORITY + 5,
    //     emc2305TaskStack_2,
    //     &emc2305TaskBuffer_2);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}
