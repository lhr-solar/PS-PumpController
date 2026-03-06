#include "pumpController.h"
#include "tasks.h"

// pump control
void PumpControl_Task(void* argument) {

    if(!FanChip_Init(PUMP, PWM_MODE)) {
        Error_Handler();
    }

    while (1) {
        // Testing Direct PWM Drive Mode
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN3, 25) != EMC2305_OK) {
            Error_Handler();
        };
        //printf("Task 2: Pump PWM drive set to 25%%\r\n");
        printf("Measured RPM for 25: %u\r\n", EMC2305_GetFanRPM(&chip, EMC2305_FAN3));
        vTaskDelay(pdMS_TO_TICKS(10000));

        // Set PWM2 duty cycle to 100%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN3, 100) != EMC2305_OK) {
            Error_Handler();
        };
        //printf("Task 2: Pump PWM drive set to 100%%\r\n");
        printf("Measured RPM for 100: %u\r\n", EMC2305_GetFanRPM(&chip, EMC2305_FAN3));
        vTaskDelay(pdMS_TO_TICKS(10000));

        // // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN3);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink pump LED
        // LED_Blink(led_configs[PUMP_LED]);
        HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
    }
}