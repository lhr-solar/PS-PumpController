#include "pumpController.h"
#include "tasks.h"

// pump control
void PumpSpeed_Task(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;
    printf("global config set\n");

    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }
    printf("Task 1: Global Config Set\r\n");
    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = false; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
    config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
    config1.range = EMC2305_RNG_2000;

    EMC2305_Fan_Config2 config2 = { 0 };
    config2.enable_ramp_rate_ctl = true;
    config2.enable_glitch_filter = true;
    config2.error_window = EMC2305_ERG_200RPM;
    config2.derivative_options = EMC2305_DPT_BOTH;

    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN3, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };
    // Depends on the fan lol (should be in fan datasheet)
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN3, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };
    // Set minimum drive to 0%
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN3, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };
    // Set PID Gain to lowest (1x)
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN3, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    // Set PWM output mode to open-drain (use false for push-pull)
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN3, false) != EMC2305_OK) {
        Error_Handler();
    };

    while (1) {
        // Testing Direct PWM Drive Mode
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN3, 25) != EMC2305_OK) {
            Error_Handler();
        };
        //printf("Task 2: Pump PWM drive set to 25%%\r\n");
        printf("Measured RPM for 25: %u\r\n", EMC2305_GetFanRPM(&chip, EMC2305_FAN3));
        vTaskDelay(pdMS_TO_TICKS(10000));

        // Set PWM2 duty cycle to 25%
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
        LED_Blink(led_configs[PUMP_LED]);
    }
}