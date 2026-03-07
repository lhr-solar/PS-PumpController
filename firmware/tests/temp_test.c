/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */
/*    TEMP TEST: PRINTS MEASURED ADC VALUE AND CONVERTED TEMP ON LOOP + BLINKY     */
/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */

#include "tasks.h"
#include "common.h"
#include "pindefs.h"

EMC2305_HandleTypeDef chip;

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[configMINIMAL_STACK_SIZE];

StaticTask_t xTempTaskBuffer;
StackType_t xTempStack[configMINIMAL_STACK_SIZE];

int main() {

    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    if (!PumpController_Init()) Error_Handler();

    // Create tasks
    xTaskCreateStatic(Init_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        INIT_TASK_PRIO,
        initTaskStack,
        &initTaskBuffer);
    
    xTaskCreateStatic(
        Blinky_Task,
        "Blinky",
        configMINIMAL_STACK_SIZE,
        NULL,
        BLINKY_TASK_PRIO,
        xBlinkyStack,
        &xBlinkyTaskBuffer
    );
    
    xTaskCreateStatic(
        Temp_Task,
        "Temp Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TEMP_TASK_PRIO,
        xTempStack,
        &xTempTaskBuffer
    );

    vTaskStartScheduler();

    return 0;
}