#include "tasks.h"

#define FANS_TASK_DELAY     pdMS_TO_TICKS(10000)

void FanControl_Task(void* argument) {
    
    if(!FanChip_Init(FAN2, PWM_MODE)) {
        Error_Handler();
    }

    while (1) {
        // Testing Direct PWM Drive Mode
        // Set PWM2 duty cycle to 25%
        // if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // printf("Task 1: PWM1 drive set to 25%%\r\n");

        // vTaskDelay(pdMS_TO_TICKS(5000));

        // Set PWM2 duty cycle to 75%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 50) != EMC2305_OK) {
            Error_Handler();
        };
        vTaskDelay(FANS_TASK_DELAY);

        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 0) != EMC2305_OK) {
            Error_Handler();
        };
        vTaskDelay(FANS_TASK_DELAY);

        // printf("Task 1: PWM2 drive set to 25%%\r\n");

        // Testing FSC Mode
        // Set RPM to 3000
        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // //printf("Task 1: Fan RPM target set to 3000\r\n");

        // vTaskDelay(pdMS_TO_TICKS(10000));

        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 100) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // //printf("Task 1: Fan RPM target set to 8000\r\n");
        // vTaskDelay(pdMS_TO_TICKS(10000));

        // // Get current rpm
        //  uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
        //  printf("Measured RPM: %u\r\n", rpm);

        // // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink fan LED
        HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);

    }
}

