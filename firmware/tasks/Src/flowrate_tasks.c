#include "tasks.h"

extern QueueHandle_t flowrate_queue;
static FlowMsg_t message;

#define FLOWRATE_TASK_DELAY      pdMS_TO_TICKS(100)

void Flowrate_Task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {

        if (Flow_GetReading(&message, xLastWakeTime) != FLOWRATE_READ_FAIL) {
            Error_Handler();
        }
        
        printf("Flowrate: %u.%u L/min\n\r", message.flowrate_x10 / 10, message.flowrate_x10 % 10);
        HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, FLOWRATE_TASK_DELAY);
    }
}