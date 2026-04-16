#include "tasks.h"

StaticTask_t xFaultTaskBuffer;
StackType_t xFaultStack[8*configMINIMAL_STACK_SIZE];

static CAN_RxHeaderTypeDef bps_header = {0};
static uint8_t bps_rx_data[8] = {0};

// void Fault_Task(EventBits_t pending){    
    
//     while(1){

//         EventBits_t pending = faultBit_wait(NUM_FAULTS, portMAX_DELAY);

//         if(pending != 0){
//             // never return
//             FaultState(pending);
//         }

//     }
// }

// void FaultState(EventBits_t pending) {
//     while(1) {
//         HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
//         EMC2305_SetFanPWM(&chip, EMC2305_FAN2, FAN_FAULT_PWM);
//         printf("FAULT STATE: pending fault bits: %u\n\r", pending);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

void CANrecv_Task() {
    bps_status_msg_t bps_status = {0};

    while(1) {
        if (can_recv(hcan1, CAN_ID_BPS_STATUS, &bps_header, bps_rx_data, CAN_TASK_DELAY) != CAN_OK) {
            UnpackBPSStatusMessage(&bps_status, bps_header.StdId, bps_rx_data);
            if(bps_status.BPS_Fault){
                printf("BPS FAULT DETECTED\n\r");
                HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
                EMC2305_SetFanPWM(&chip, EMC2305_FAN2, FAN_FAULT_PWM);
            }
            
            HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
        }

    }
}