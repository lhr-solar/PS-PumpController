#include "tasks.h"

#define CAN_RECV_TASK_DELAY     pdMS_TO_TICKS(10)

StaticTask_t xCANrecvTaskBuffer;
StackType_t xCANrecvStack[configMINIMAL_STACK_SIZE];


void CANrecv_Task(void* argument) {
    static CAN_RxHeaderTypeDef bps_header = {0};
    static uint8_t bps_rx_data[8] = {0};
    bps_status_msg_t local_bps_status = {0};
    
    // xBpsDataMutex = xSemaphoreCreateMutexStatic();

    while(1) {
        // Blocks until a CAN message is received
        if (can_recv(hcan1, CAN_ID_BPS_STATUS, &bps_header, bps_rx_data, CAN_TASK_DELAY) == CAN_OK) {
            UnpackBPSStatusMessage(&local_bps_status, bps_header.StdId, bps_rx_data);
            
            // Update thread-safe global storage
            UpdateGlobalBPS(&local_bps_status);

            // Handle safety faults immediately
            if(local_bps_status.BPS_Fault){
                printf("BPS FAULT DETECTED\n\r");
                HAL_GPIO_WritePin(TEMP_LED_PORT, TEMP_LED_PIN, GPIO_PIN_SET);
                EMC2305_SetFanPWM(&chip, EMC2305_FAN1, FAN_FAULT_PWM);
                EMC2305_SetFanPWM(&chip, EMC2305_FAN2, FAN_FAULT_PWM);

                Error_Handler();    // TODO: CHANGE THIS??

            } else {
                HAL_GPIO_WritePin(TEMP_LED_PORT, TEMP_LED_PIN, GPIO_PIN_RESET);
            }
            
            // HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
        }
        vTaskDelay(CAN_RECV_TASK_DELAY);
    }
}