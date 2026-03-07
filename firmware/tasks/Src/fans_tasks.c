#include "tasks.h"

#define FANS_TASK_DELAY     pdMS_TO_TICKS(10000)

void FanControl_Task(void* argument) {
    
    if(FanChip_Init(FAN2, PWM_MODE) != FAN_CHIP_OK) {
        Error_Handler();
    }

    while (1) {
        #if FAN_MODE == FAN_MODE_PWM

            EMC2305_SetFanPWM(&chip, EMC2305_FAN2, FAN_TEST_PWM);

        #elif FAN_MODE == FAN_MODE_FSC

            EMC2305_SetFanRPM(&chip, EMC2305_FAN2, FAN_TEST_RPM);

        #endif

        HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
        vTaskDelay(FANS_TASK_DELAY);

    }
}

