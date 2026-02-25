#include "pumpController.h"
#include "tasks.h"


extern I2C_HandleTypeDef hi2c1;
EMC2305_HandleTypeDef chip;
// StaticTask_t initTaskBuffer;
// StackType_t initTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_1;
StackType_t emc2305TaskStack_1[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_2;
StackType_t emc2305TaskStack_2[configMINIMAL_STACK_SIZE];

// void Init_Task(void* argument) {
//     // Init UART printf
//     husart1->Init.BaudRate = 115200;
//     husart1->Init.WordLength = UART_WORDLENGTH_8B;
//     husart1->Init.StopBits = UART_STOPBITS_1;
//     husart1->Init.Parity = UART_PARITY_NONE;
//     husart1->Init.Mode = UART_MODE_TX_RX;
//     husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
//     husart1->Init.OverSampling = UART_OVERSAMPLING_16;

//     printf_init(husart1);
    
//     // Initialize EMC2305
//     // Only call from ONE task!
//     if (EMC2305_Init(&chip, &hi2c1, 0x4D) != EMC2305_OK) {
//         Error_Handler();
//     }
//     printf("EMC2305 Initialized\r\n");
//     // Task kills itself
//     vTaskDelete(NULL);
// }

// fan control
void FanSpeed_Task(void* argument) {
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

    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };
    // Depends on the fan lol (should be in fan datasheet)
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN2, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };
    // Set minimum drive to 0%
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };
    // Set PID Gain to lowest (1x)
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    // Set PWM output mode to open-drain (use false for push-pull)
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN2, true) != EMC2305_OK) {
        Error_Handler();
    };

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
        vTaskDelay(pdMS_TO_TICKS(10000));

        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 0) != EMC2305_OK) {
            Error_Handler();
        };
        vTaskDelay(pdMS_TO_TICKS(10000));

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
        LED_Blink(led_configs[FANCHIP_LED]);
        //HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);

    }
}

