#include "tasks.h"
#include "PumpController.h"

extern EMC2305_HandleTypeDef chip;

#define BLINKY_TASK_DELAY    pdMS_TO_TICKS(500)

void Init_Task(void* argument) {
    // Init UART printf
    husart1->Instance = USART1;
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;
    husart1->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    husart1->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(husart1) != HAL_OK)
    {
    Error_Handler();
    }

    printf_init(husart1);
    
    // Initialize EMC2305
    // Only call from ONE task!
    if (EMC2305_Init(&chip, &hi2c1, 0x4D) != EMC2305_OK) {
        Error_Handler();
    }
    printf("EMC2305 Initialized\r\n");
    HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);

    vTaskDelete(NULL);
}

void Blinky_Task(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);
        vTaskDelay(BLINKY_TASK_DELAY);
    }
}