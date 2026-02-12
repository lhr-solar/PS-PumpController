// Test for the EMC2305 Fan Chip Driver
// Spawns 2 tasks to test thread safe control of underlying I2C bus

#include "UART.h"
#include "common.h"
#include "pumpController.h"
#include "pindefs.h"

#include <stdio.h>

// From datasheet
#define DEFAULT_DEV_ADDR 0x4D

// LED pins for PSOM
#define STATUS_LED_PORT GPIOA
#define STATUS_LED_PIN_1 GPIO_PIN_7
#define STATUS_LED_PIN_2 GPIO_PIN_8
#define STATUS_LED_PIN_3 GPIO_PIN_15

// USART pins for PSOM
#define USART_PORT GPIOA
#define USART_TX_PIN GPIO_PIN_9
#define USART_RX_PIN GPIO_PIN_10

// I2C pins for PSOM
#define I2C_PORT GPIOB
#define I2C_SCL_PIN GPIO_PIN_6
#define I2C_SDA_PIN GPIO_PIN_7

extern I2C_HandleTypeDef hi2c1;
EMC2305_HandleTypeDef chip;

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_1;
StackType_t emc2305TaskStack_1[configMINIMAL_STACK_SIZE];

void mx_uart_init(void) {
    // UART init
    GPIO_InitTypeDef InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    InitStruct.Pin = USART_TX_PIN | USART_RX_PIN;
    InitStruct.Mode = GPIO_MODE_AF_PP;
    InitStruct.Pull = GPIO_NOPULL;
    InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(USART_PORT, &InitStruct);
    printf("UART Initialized\r\n");
}

// void mx_i2c_init(void) {
//     // initialize I2C pins on PSOM
//     GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//     RCC_PeriphCLKInitTypeDef ClkInit = { 0 };
//     /* USER CODE BEGIN I2C1_MspInit 0 */

//     /* USER CODE END I2C1_MspInit 0 */

//     /** Initializes the peripherals clock
//     */
//     ClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
//     ClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
//     if (HAL_RCCEx_PeriphCLKConfig(&ClkInit) != HAL_OK)
//     {
//         Error_Handler();
//     }

//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     /**I2C1 GPIO Configuration
//     PB6     ------> I2C1_SCL
//     PB7     ------> I2C1_SDA
//     */
//     GPIO_InitStruct.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//     HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);

//     // I2C Interrupt Init
//     HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
//     HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
//     HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
//     HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

//     /* Peripheral clock enable */
//     __HAL_RCC_I2C1_CLK_ENABLE();

//     // MX I2C Init
//     hi2c1.Instance = I2C1;
//     hi2c1.Init.Timing = 0x10D19CE4;
//     hi2c1.Init.OwnAddress1 = 0;
//     hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//     hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//     hi2c1.Init.OwnAddress2 = 0;
//     hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//     hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//     hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//     if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//     {
//         Error_Handler();
//     }

//     /** Configure Analogue filter
//     */
//     if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//     {
//         Error_Handler();
//     }

//     /** Configure Digital filter
//     */
//     if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//     {
//         Error_Handler();
//     }
// }

// Initialize UART and EMC2305
void Init_Task(void* argument) {
    // Init UART printf
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);

    // Initialize EMC2305
    // Only call from ONE task!
    if (EMC2305_Init(&chip, &hi2c1, DEFAULT_DEV_ADDR) != EMC2305_OK) {
        Error_Handler();
    }

    // Task kills itself
    vTaskDelete(NULL);
}

void EMC2305_Task_1(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);

    printf("Task 1: EMC2305 Initialized\r\n");

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;
    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }

    printf("Task 1: Global Config Set\r\n");

    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = true; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
    config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
    config1.range = EMC2305_RNG_2000;
    EMC2305_Fan_Config2 config2 = { 0 };
    config2.enable_ramp_rate_ctl = true;
    config2.enable_glitch_filter = true;
    config2.error_window = EMC2305_ERG_200RPM;
    config2.derivative_options = EMC2305_DPT_BOTH;
    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN1, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };

    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Fan 2 Config Set\r\n");

    // Depends on the fan lol
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN2, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: PWM Frequency set to 19.53 kHz\r\n");

    // Set minimum drive to 0%
    // FUCK MICROCHIP THEY NEED TO KILL THEMSELVES
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Minimum Drive set to 0\r\n");

    // Set PID Gain to lowest (1x)
    // I HATE THESE BOZOS WHY IS THIS NOT THE DEFAULT
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: PID Gain set to 1x\r\n");

    // Set PWM output mode to open-drain
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN2, true) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Fan 2 output set to open-drain\r\n");

    while (1) {
        // Testing Direct PWM Drive Mode
        // Set PWM2 duty cycle to 25%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 1: PWM1 drive set to 25%%\r\n");
        
        vTaskDelay(pdMS_TO_TICKS(5000));


        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 75) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 1: PWM2 drive set to 75%%\r\n");

        // Testing FSC Mode
        // Set RPM to 3000
        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // printf("Task 1: RPM target set to 3000\r\n");

        //         vTaskDelay(pdMS_TO_TICKS(5000));

        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 8000) != EMC2305_OK) {
        //     Error_Handler();
        // };

        // Get current rpm
         uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
         printf("Measured RPM: %u\r\n", rpm);

        // Get current pwm
        uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        printf("Drive PWM: %u\r\n", pwm);

        // Blink Heartbeat LED
        HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void) {
    // initialize the HAL and system clock
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    //HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init peripherals
    mx_uart_init();
    MX_I2C1_Init();
    LEDs_Init();
        LED_On(led_configs[PUMP_LED]);

    //HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
        LED_On(led_configs[TEMP_LED]);

    xTaskCreateStatic(Init_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);

    xTaskCreateStatic(EMC2305_Task_1,
        "EMC2305 Task 1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        emc2305TaskStack_1,
        &emc2305TaskBuffer_1);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}
