#include "tasks.h"

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
            // Convert data to current measurent
            message.temp_data = ADCToTemp(message.adc_val);
            printf("ADC Value: %u, Temp: %d\n\r", message.adc_val, message.temp_data);
        }
        
        HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}