#include "Fan_I2C.h"

I2C_HandleTypeDef hi2c1;
extern EMC2305_HandleTypeDef chip;

#define EMC_POWER_ON_TIME       pdMS_TO_TICKS(250)

// Initializes and configs I2C1 pins PB6 and PB7 for PSOM
FanChip_Status_t MX_I2C1_Init(void) {
    // initialize I2C pins on PSOM
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef ClkInit = { 0 };

    // Initializes the peripherals clock
    ClkInit.PeriphClockSelection |= RCC_PERIPHCLK_I2C1;
    ClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;

    if (HAL_RCCEx_PeriphCLKConfig(&ClkInit) != HAL_OK)
    {
        return FAN_CHIP_INIT_FAIL;
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
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C_INTERRUPT_PRIO, 0);
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
        return FAN_CHIP_INIT_FAIL;
    }

    // Configure Analog filter
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        return FAN_CHIP_INIT_FAIL;
    }

    // Configure Digital filter
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        return FAN_CHIP_INIT_FAIL;
    }

    return FAN_CHIP_OK;

}

// I2C Event Handler Callback
void I2C1_EV_IRQHandler(void) {
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

// I2C Error Handler Callback
void I2C1_ER_IRQHandler(void) {
    HAL_I2C_ER_IRQHandler(&hi2c1);
}

// I2C Transmit Interrupt Callback
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterTxCpltCallback(hi2c);
}

// I2C Receive Interrupt Callback
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterRxCpltCallback(hi2c);
}

FanChip_Status_t FanChip_Init(FanChip_Device_t device, FanChip_Mode_t mode) {
    // Allow chip to power on
    vTaskDelay(EMC_POWER_ON_TIME);

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;

    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    }
    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = (mode == FSC_MODE);    // true if using FSC (Closed Loop RPM Control),  false for pwm directly
    config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
    config1.range = EMC2305_RNG_2000;

    EMC2305_Fan_Config2 config2 = { 0 };
    config2.enable_ramp_rate_ctl = true;
    config2.enable_glitch_filter = true;
    config2.error_window = EMC2305_ERG_200RPM;
    config2.derivative_options = EMC2305_DPT_BOTH;

    if (EMC2305_SetFanConfig(&chip, device, &config1, &config2) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    };
    // Depends on the fan (should be in datasheet)
    if (EMC2305_SetPWMBaseFrequency(&chip, device, EMC2305_PWM_19k53) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    };
    // Set minimum drive to 0%
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(device, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    };
    // Set PID Gain to lowest (1x)
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(device, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    };

    // Set PWM output mode to open-drain (use false for push-pull)
    if (EMC2305_SetPWMOutputMode(&chip, device, true) != EMC2305_OK) {
        return FAN_CHIP_INIT_FAIL;
    };

    return FAN_CHIP_OK;
}

FanChip_Status_t Cooling_Init(void) {
    #if FAN_MODE == FAN_MODE_PWM
        if (FanChip_Init(FAN2, PWM_MODE) != FAN_CHIP_OK)
            return FAN_CHIP_INIT_FAIL;
    #elif FAN_MODE == FAN_MODE_FSC
        if (FanChip_Init(FAN2, FSC_MODE) != FAN_CHIP_OK)
            return FAN_CHIP_INIT_FAIL;
    #endif

    #if PUMP_MODE == FAN_MODE_PWM
        if (FanChip_Init(PUMP, PWM_MODE) != FAN_CHIP_OK)
            return FAN_CHIP_INIT_FAIL;
    #endif
    return FAN_CHIP_OK;
}