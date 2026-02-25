#include "EMC2305.h"
#include "pumpController.h"
#include "stm32xx_hal.h"

I2C_HandleTypeDef hi2c1;

// Initializes and configs I2C1 pins PB6 and PB7 for PSOM
bool MX_I2C1_Init(void) {
    // initialize I2C pins on PSOM
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef ClkInit = { 0 };

    // Initializes the peripherals clock
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

    return true;

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
