#include "tasks.h"

#define PUMP_TASK_DELAY     pdMS_TO_TICKS(10000)

// pump control
void PumpControl_Task(void* argument) {

    if(!FanChip_Init(PUMP, PWM_MODE)) {
        Error_Handler();
    }

    while (1) {
        EMC2305_SetFanPWM(&chip, EMC2305_FAN3, PUMP_TEST_PWM);
        vTaskDelay(PUMP_TASK_DELAY);

        // Blink pump LED
        HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
    }
}