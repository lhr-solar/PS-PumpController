// #include "EMC2305.h"
// #include "pumpController.h"
// #include "stm32xx_hal.h"

// I2C_HandleTypeDef hi2c1;
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

// void EMC2305_Task_1(void* argument) {
//     // Allow chip to power on
//     vTaskDelay(pdMS_TO_TICKS(250));

//     // Set global config
//     EMC2305_Global_Config config = { 0 };
//     config.watchdog_enable = true;

//     if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
//         Error_Handler();
//     }

//     // Set config1 and config2
//     EMC2305_Fan_Config1 config1 = { 0 };
//     config1.enable_closed_loop = false; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
//     config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
//     config1.range = EMC2305_RNG_2000;

//     EMC2305_Fan_Config2 config2 = { 0 };
//     config2.enable_ramp_rate_ctl = true;
//     config2.enable_glitch_filter = true;
//     config2.error_window = EMC2305_ERG_200RPM;
//     config2.derivative_options = EMC2305_DPT_BOTH;

//     if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Depends on the fan lol (should be in fan datasheet)
//     if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN2, EMC2305_PWM_19k53) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set minimum drive to 0%
//     if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set PID Gain to lowest (1x)
//     if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Set PWM output mode to open-drain (use false for push-pull)
//     if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN2, true) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Control with direct PWM
//     // Set PWM2 duty cycle to 25%
//     if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
//         Error_Handler();
//     };

//     // Control with closed-loop FSC
//     // Set RPM to 3000
//     if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
//         Error_Handler();
//     };
// }

// int main(void) {
//     // Init your HAL, System Clock, and Peripherals here

//     // Create tasks
//     xTaskCreateStatic(Init_Task,
//         "Init Task",
//         configMINIMAL_STACK_SIZE,
//         NULL,
//         tskIDLE_PRIORITY + 1,
//         initTaskStack,
//         &initTaskBuffer);

//     xTaskCreateStatic(EMC2305_Task_1,
//         "EMC2305 Task 1",
//         configMINIMAL_STACK_SIZE,
//         NULL,
//         tskIDLE_PRIORITY + 2,
//         emc2305TaskStack_1,
//         &emc2305TaskBuffer_1);

//     vTaskStartScheduler();

//     while (1) {
//     }

//     return 0;
// }

// // I2C Transmit Interrupt Callback
// void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
//     EMC2305_I2C_MasterTxCpltCallback(hi2c);
// }

// // I2C Receive Interrupt Callback
// void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
//     EMC2305_I2C_MasterTxCpltCallback(hi2c);
// }
