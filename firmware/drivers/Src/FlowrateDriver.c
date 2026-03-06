#include "FlowrateDriver.h"

TIM_HandleTypeDef htim2;

#define FLOW_ITEM_SIZE sizeof(uint32_t)
#define FLOW_QUEUE_LENGTH 10

QueueHandle_t flowrate_queue;
uint8_t flowrate_qStorage[FLOW_QUEUE_LENGTH * FLOW_ITEM_SIZE];
static StaticQueue_t xStaticQueue_flowrate;

Flow_Status_t MX_TIM2_Init(void) {

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_3;        // PB3 = TIM2_CH2
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;      // change to PULLUP if needed
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    flowrate_queue = xQueueCreateStatic(
        FLOW_QUEUE_LENGTH,
        FLOW_ITEM_SIZE,
        flowrate_qStorage,
        &xStaticQueue_flowrate
    );

    if (flowrate_queue == NULL)
        Error_Handler();

    HAL_NVIC_SetPriority(TIM2_IRQn,
                         configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
                         0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 79;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
        Error_Handler();

    TIM_IC_InitTypeDef sConfigIC = {0};
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;

    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
        Error_Handler();

    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);

    return FLOWRATE_OK;
}


void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    static uint32_t last_capture = 0;
    uint32_t current_capture;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (htim->Instance == TIM2 &&
        htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {

        current_capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

        uint32_t diff = current_capture - last_capture;
        last_capture = current_capture;

        if (diff != 0) {
            xQueueSendFromISR(
                flowrate_queue,
                &diff,
                &xHigherPriorityTaskWoken
            );

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}