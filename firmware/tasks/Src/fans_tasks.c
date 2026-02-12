// #include "pumpController.h"

// EMC2305_HandleTypeDef chip;

// void Init_Task(void* argument) {
//     // Initialize EMC2305
//     // Only call from ONE task!
//     if (EMC2305_Init(&chip, &hi2c1, 0x4D) != EMC2305_OK) {
//         Error_Handler();
//     }

//     // Task kills itself
//     vTaskDelete(NULL);
// }

// void FanSpeed_Task(void* argument) {
//     // Allow chip to power on
//     vTaskDelay(pdMS_TO_TICKS(250));

//     // Set global config
//     EMC2305_Global_Config config = { 0 };
//     config.watchdog_enable = true;

//     if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
//         Error_Handler();
//     }

//     // Set config1 and config2
//     EMC2305_Fan_Config1 cfg1 = {
//     .enable_closed_loop = true,
//     .range = EMC2305_RNG_2000,
//     .edges = EMC2305_EDG_5,
//     .update_time = EMC2305_UDT_100,
//     };

//     EMC2305_Fan_Config2 cfg2 = {
//         .enable_ramp_rate_ctl = true,
//         .enable_glitch_filter = true,
//         .derivative_options = EMC2305_DPT_BOTH,
//         .error_window = EMC2305_ERG_200RPM,
//     };

//     if (EMC2305_SetFanConfig(&chip, EMC2305_FAN1, &cfg1, &cfg2) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Depends on the fan lol (should be in fan datasheet)
//     if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN1, EMC2305_PWM_19k53) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set minimum drive to 0%
//     if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN1, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set PID Gain to lowest (1x)
//     if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN1, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set PWM output mode to open-drain (use false for push-pull)
//     if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN1, true) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Control with direct PWM
//     // Set PWM2 duty cycle to 25%
//     // if (EMC2305_SetFanPWM(&chip, EMC2305_FAN1, 25) != EMC2305_OK) {
//     //      Error_Handler();
//     //  };

//     // Control with closed-loop FSC
//     // Set RPM to 3000
//     if (EMC2305_SetFanRPM(&chip, EMC2305_FAN1, 3000) != EMC2305_OK) {
//         Error_Handler();
//     };
// }