#include "tasks.h"

#define TEMP_TASK_DELAY      pdMS_TO_TICKS(100)

void Temp_Task(void *pvParameters) {
    TempMsg_t message;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Start ADC reading
        // Reset queue to prevent race condition (data already in queue and task does not wake up)
        if (Temp_StartADC(true) != TEMP_OK) {
            Error_Handler();
        };

        // Block until we receive data in queue
        if (Temp_GetReading(&message, portMAX_DELAY) == TEMP_OK) {
            printf("ADC: %u Temp: %ld.%03ld C\n", message.adc_val, GetTempInt(message.temp_data), GetTempFrac(message.temp_data));
        }
        
        HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, TEMP_TASK_DELAY);
    }
}