#include "EMC2305.h"
#include "pumpController.h"
#include "stm32xx_hal.h"

EMC2305_HandleTypeDef chip;
I2C_HandleTypeDef hi2c1;

// Initializes and configs I2C1 pins PB6 and PB7 for PSOM
void MX_I2C1_Init(void) {
    // initialize I2C pins on PSOM
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef ClkInit = { 0 };
    /* USER CODE BEGIN I2C1_MspInit 0 */

    /* USER CODE END I2C1_MspInit 0 */

    /** Initializes the peripherals clock
    */
    ClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    ClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&ClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = FAN_SCL_PIN | FAN_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(FAN_I2C_PORT, &GPIO_InitStruct);

    // I2C Interrupt Init
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    // MX I2C Init
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x10D19CE4;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    // Configure Analog filter
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    // Configure Digital filter
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }

}


// Handles I2C1 event interrupt
void I2C1_EV_IRQHandler(void) {
    LED_Blink(led_configs[FANCHIP_LED]);
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

// Handles I2C1 error interrupt
void I2C1_ER_IRQHandler(void) {
    LED_On(led_configs[FANCHIP_LED]);
    HAL_I2C_ER_IRQHandler(&hi2c1);
}

void Init_Task(void* argument) {
    // Initialize EMC2305
    // Only call from ONE task!
    if (EMC2305_Init(&chip, &hi2c1, 0x4D) != EMC2305_OK) {
        Error_Handler();
    }

    // Task kills itself
    vTaskDelete(NULL);
}

void EMC2305_Task_1(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;

    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }

    // Set config1 and config2
    EMC2305_Fan_Config1 cfg1 = {
    .enable_closed_loop = true,
    .range = EMC2305_RNG_2000,
    .edges = EMC2305_EDG_5,
    .update_time = EMC2305_UDT_100,
    };

    EMC2305_Fan_Config2 cfg2 = {
        .enable_ramp_rate_ctl = true,
        .enable_glitch_filter = true,
        .derivative_options = EMC2305_DPT_BOTH,
        .error_window = EMC2305_ERG_200RPM,
    };

    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN1, &cfg1, &cfg2) != EMC2305_OK) {
        Error_Handler();
    };

    // Depends on the fan lol (should be in fan datasheet)
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN1, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };

    // Set minimum drive to 0%
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN1, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    // Set PID Gain to lowest (1x)
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN1, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    // Set PWM output mode to open-drain (use false for push-pull)
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN1, true) != EMC2305_OK) {
        Error_Handler();
    };

    // Control with direct PWM
    // Set PWM2 duty cycle to 25%
    // if (EMC2305_SetFanPWM(&chip, EMC2305_FAN1, 25) != EMC2305_OK) {
    //     Error_Handler();
    // };

    // Control with closed-loop FSC
    // Set RPM to 3000
    if (EMC2305_SetFanRPM(&chip, EMC2305_FAN1, 3000) != EMC2305_OK) {
        Error_Handler();
    };
}

// I2C Transmit Interrupt Callback
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterTxCpltCallback(hi2c);
}

// I2C Receive Interrupt Callback
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterTxCpltCallback(hi2c);
}
