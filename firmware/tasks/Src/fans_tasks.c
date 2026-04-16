#include "tasks.h"

#define FANS_TASK_DELAY     pdMS_TO_TICKS(100)

// typedef struct {
//     uint16_t Radiator_Fan_Speed_Measurement_1;
//     uint16_t Radiator_Fan_Speed_Measurement_2;
//     uint16_t Radiator_Fan_Speed_Target_1;
//     uint16_t Radiator_Fan_Speed_Target_2;
// } radiator_fanspeed_t; // in RPM

static CAN_TxHeaderTypeDef fans_header = {0};
static uint8_t fans_tx_data[8] = {0};

void FanControl_Task(void* argument) {
    
    radiator_fanspeed_t FanMsg = {0};

    if(Cooling_Init() != FAN_CHIP_OK) {
        Error_Handler();
    }

    while (1) {

        #if FAN_MODE == FAN_MODE_PWM

            EMC2305_SetFanPWM(&chip, EMC2305_FAN2, FAN_TEST_PWM);
            FanMsg.Radiator_Fan_Speed_Target_2 = FAN_TEST_PWM_TARGET;
            FanMsg.Radiator_Fan_Speed_Measurement_2 = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);

        #elif FAN_MODE == FAN_MODE_FSC

            EMC2305_SetFanRPM(&chip, EMC2305_FAN2, FAN_TEST_RPM);
            FanMsg.Radiator_Fan_Speed_Target_2 = FAN_TEST_RPM;
            FanMsg.Radiator_Fan_Speed_Measurement_2 = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);

        #endif

        printf("target pwm: %d\n\r", FAN_TEST_PWM_TARGET);
        PackFanCANMessage(&fans_header, &FanMsg, fans_tx_data);
        if (can_send(hcan1, &fans_header, fans_tx_data, CAN_TASK_DELAY) != CAN_OK) {
            HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
        }
        printf("fan message:");
        for(int i = 0; i < 8 ; i++) {
            printf(" %u", fans_tx_data[i]);
        }
        printf("\n\r");

        HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);


        vTaskDelay(FANS_TASK_DELAY);

    }
}

