#include "tasks.h"

#define TEMP_TASK_DELAY      pdMS_TO_TICKS(100)

void Temp_Task(void *pvParameters) {
    TempMsg_t message;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    static CAN_TxHeaderTypeDef temp_header = {0};
    static uint8_t temp_tx_data[8];

    // if (CAN_Init() != CAN_INIT_OK){
    //     HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
    //     Error_Handler();
    // }
    while (1) {
        // Start ADC reading
        // Reset queue to prevent race condition (data already in queue and task does not wake up)
        HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
        if (Temp_StartADC(true) != TEMP_OK) {
            printf("b\n\r");
        };
        // Block until we receive data in queue
        if (Temp_GetReading(&message, portMAX_DELAY) == TEMP_OK) {
            printf("ADC: %u Temp: %ld.%03ld C\n\r", message.adc_val, GetTempInt(message.temp_data), GetTempFrac(message.temp_data));
            PackTempCANMessage(&temp_header, &message, temp_tx_data);
            HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);
            // can_send(hcan1, &temp_header, temp_tx_data, portMAX_DELAY);
            if (can_send(hcan1, &temp_header, temp_tx_data, TEMP_TASK_DELAY) != CAN_OK) {
                HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
            }
        }
        
        HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime,TEMP_TASK_DELAY);
        
    }
}