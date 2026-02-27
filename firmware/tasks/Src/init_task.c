#include "pumpController.h"
#include "tasks.h"

void Init_Task(void* argument) {
    // Init UART printf
    husart1->Init.BaudRate = BAUD_RATE;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);
    
    // Initialize EMC2305
    // Only call from ONE task!
    if (EMC2305_Init(&chip, &hi2c1, 0x4D) != EMC2305_OK) {
        Error_Handler();
    }
    printf("EMC2305 Initialized\r\n");
    // Task kills itself
    vTaskDelete(NULL);
}