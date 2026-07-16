/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */
/*        PID LOOP FOR PUMP, SCALE RADIATOR FAN PERCENTAGE TO PUMP SPEED 1:2       */
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

    if (MUTEX_Init() != MUTEX_INIT_OK) Error_Handler();
    
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

    xTaskCreateStatic(PumpControlLoop_Task,
        "Pump Control Task",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        PUMP_TASK_PRIO,
        PumpControlStack,
        &PumpControlTaskBuffer);

    xTaskCreateStatic(FanControlLoop_Task,
        "Fan Control Task",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        FAN_TASK_PRIO,
        FanControlStack,
        &FanControlTaskBuffer);
    
    xTaskCreateStatic(CANrecv_Task,
        "CANrecv Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        CAN_RECV_TASK_PRIO,
        xCANrecvStack,
        &xCANrecvTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}