#include "tasks.h"

#define PUMP_TASK_DELAY             pdMS_TO_TICKS(10000)
#define FLOWRATE_TASK_DELAY         pdMS_TO_TICKS(100)

extern QueueHandle_t flowrate_queue;
static FlowMsg_t message;
static CAN_TxHeaderTypeDef flow_header = {0};
static uint8_t flow_tx_data[8] = {0};

// pump control basic
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


// ---------------------------------------------------------------- //

// pump control with pid
void PumpControlLoop_Task(void* argument) {
    pump_status_t pump_status_msg = {0};
    bps_status_msg_t current_bps = {0};
    FlowMsg_t message = {0};
    CAN_TxHeaderTypeDef flow_header = {0};
    uint8_t flow_tx_data[8] = {0};
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    PID_Controller pump_pid;
    PID_Init(&pump_pid, KP, KI, KD, PUMP_TEMP_SETPOINT, PUMP_PWM_MIN, PUMP_PWM_MAX, PUMP_SAMPLING_TIME);

    if(Cooling_Init() != FAN_CHIP_OK) {
        pump_status_msg.Pump_Fault = PUMP_CONTROLLER_INIT_FAIL;
        Error_Handler();
    }
    pump_status_msg.Pump_Fault = PUMP_CONTROLLER_OK;

    while (1) {
        ReadGlobalBPS(&current_bps);
        
        float temp = (float)current_bps.Main_Battery_Avg_Temperature;
        uint8_t calculated_pwm = (uint8_t)PID_Update(&pump_pid, temp);
        
        EMC2305_SetFanPWM(&chip, EMC2305_FAN3, calculated_pwm);
        SetPumpDutyCycle(calculated_pwm); // Send calculation to shared state
        
        pump_status_msg.Pump_DutyCycle = calculated_pwm;
        HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
        
        if (Flow_GetReading(&message, xLastWakeTime) != FLOWRATE_READ_FAIL) {
            pump_status_msg.FlowRate_1 = message.flowrate_x10;
        } else {
            HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        }
        
        PackFlowrateCANMessage(&flow_header, &pump_status_msg, flow_tx_data);
        can_send(hcan1, &flow_header, flow_tx_data, CAN_TASK_DELAY);

        vTaskDelayUntil(&xLastWakeTime, FLOWRATE_TASK_DELAY);
    }
}