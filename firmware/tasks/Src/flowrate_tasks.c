#include "pumpController.h"
#include "tasks.h"

extern QueueHandle_t flowrate_queue;
static FlowMsg_t message;

void Flowrate_Task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {

        if (xQueueReceive(flowrate_queue, &message.diff, portMAX_DELAY)) {
            
            message.freq = 1000000 / message.diff;
            message.flowrate_x10 = (message.freq * 10) / 11;
            
        }
        
        printf("Flowrate: %u.%u L/min\n\r", message.flowrate_x10 / 10, message.flowrate_x10 % 10);
        HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}