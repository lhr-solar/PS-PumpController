/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */
/* PUMP_FAN TEST: SETS PUMP TO 25% THEN 100% AND FAN TO 3000 THEN 8000 RPM ON LOOP */
/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */

#include "tasks.h"
#include "common.h"
#include "pindefs.h"

EMC2305_HandleTypeDef chip;
extern I2C_HandleTypeDef hi2c1;

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t PumpControlTaskBuffer;
StackType_t PumpControlStack[configMINIMAL_STACK_SIZE];

StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[configMINIMAL_STACK_SIZE];

StaticTask_t FanControlTaskBuffer;
StackType_t FanControlStack[configMINIMAL_STACK_SIZE];

int main(void) {
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    if (PumpController_Init() != PUMP_CONTROLLER_OK) Error_Handler();
    
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

    xTaskCreateStatic(PumpControl_Task,
        "Pump Control Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        PUMP_TASK_PRIO,
        PumpControlStack,
        &PumpControlTaskBuffer);

    xTaskCreateStatic(FanControl_Task,
        "Fan Control Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        FAN_TASK_PRIO,
        FanControlStack,
        &FanControlTaskBuffer);
    
    xTaskCreateStatic(CANrecv_Task,
        "CANrecv Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        FAULT_TASK_PRIO,
        xFaultStack,
        &xFaultTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}