#include "pumpController.h"
#include "task.h"

EMC2305_HandleTypeDef chip;
extern I2C_HandleTypeDef hi2c1;

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t PumpControlTaskBuffer;
StackType_t PumpControlStack[configMINIMAL_STACK_SIZE];

StaticTask_t FlowrateTaskBuffer;
StackType_t FlowrateStack[configMINIMAL_STACK_SIZE];

StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[ 200 ];

void Task_Blinky(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_Init();
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init peripherals
    
    if (!PumpController_Init()) Error_Handler();
    HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);

    // LED_Blink(led_configs[STATUS_LED]);
    // Create tasks
    xTaskCreateStatic(Init_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);
    
    xTaskCreateStatic(
        Task_Blinky,
        "Blinky",
        configMINIMAL_STACK_SIZE,
        (void*) 1,
        tskIDLE_PRIORITY+3,
        xBlinkyStack,
        &xBlinkyTaskBuffer
    );

    xTaskCreateStatic(PumpControl_Task,
        "Pump Control Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 5,
        PumpControlStack,
        &PumpControlTaskBuffer);

    xTaskCreateStatic(Flowrate_Task,
        "Flowrate Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 5,
        FlowrateStack,
        &FlowrateTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}