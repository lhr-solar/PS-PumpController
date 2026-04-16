#include "tasks.h"

#define PUMP_TASK_DELAY             pdMS_TO_TICKS(10000)
#define FLOWRATE_TASK_DELAY         pdMS_TO_TICKS(100)

extern QueueHandle_t flowrate_queue;
static FlowMsg_t message;
static CAN_TxHeaderTypeDef flow_header = {0};
static uint8_t flow_tx_data[8] = {0};

// pump control
void PumpControl_Task(void* argument) {

    pump_status_t pump_status_msg = {0};
    TickType_t xLastWakeTime = xTaskGetTickCount();

    if(Cooling_Init() != FAN_CHIP_OK) {
        pump_status_msg.Pump_Fault = PUMP_CONTROLLER_INIT_FAIL;
        Error_Handler();
    }
    pump_status_msg.Pump_Fault = PUMP_CONTROLLER_OK;

    while (1) {
        EMC2305_SetFanPWM(&chip, EMC2305_FAN3, PUMP_TEST_PWM);
        pump_status_msg.Pump_DutyCycle = PUMP_TEST_PWM;

        HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
        
        if (Flow_GetReading(&message, xLastWakeTime) == FLOWRATE_READ_FAIL) {
            HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        }
        
        printf("Flowrate: %u.%u L/min\n\r", message.flowrate_x10 / 10, message.flowrate_x10 % 10);
        pump_status_msg.FlowRate_1 = message.flowrate_x10;
        HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);

        PackFlowrateCANMessage(&flow_header, &pump_status_msg, flow_tx_data);
            
        if (can_send(hcan1, &flow_header, flow_tx_data, CAN_TASK_DELAY) != CAN_OK) {
            HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);
            // Error_Handler();
        }

        vTaskDelayUntil(&xLastWakeTime, FLOWRATE_TASK_DELAY);
        // vTaskDelay(PUMP_TASK_DELAY);
    }
}