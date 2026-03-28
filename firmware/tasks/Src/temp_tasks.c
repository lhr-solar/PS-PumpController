#include "tasks.h"

#define TEMP_TASK_DELAY      pdMS_TO_TICKS(100)

void Temp_Task(void *pvParameters) {
    TempMsg_t message;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    static CAN_TxHeaderTypeDef temp_header = {0};
    static uint8_t temp_tx_data[8];

    while (1) {
        // Start ADC reading
        if (Temp_StartADC(true) != TEMP_OK) {
            Error_Handler();
        };
        // Block until we receive data in queue
        if (Temp_GetReading(&message, portMAX_DELAY) == TEMP_OK) {
            printf("ADC: %u Temp: %ld.%03ld C\n\r", message.adc_val, GetTempInt(message.temp_data), GetTempFrac(message.temp_data));
            
            PackTempCANMessage(&temp_header, &message, temp_tx_data);
            
            if (can_send(hcan1, &temp_header, temp_tx_data, CAN_TASK_DELAY) != CAN_OK) {
                HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
            }
            printf("fan message:");
        for(int i = 0; i < 8 ; i++) {
            printf(" %u", temp_tx_data[i]);
        }
        printf("\n\r");
        }
        
        HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime,TEMP_TASK_DELAY);
        
    }
}