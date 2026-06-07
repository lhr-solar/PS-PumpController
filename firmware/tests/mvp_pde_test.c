#include "tasks.h"
#include "tempTable100k.h"

StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[configMINIMAL_STACK_SIZE];

StaticTask_t xMVPTestTaskBuffer;
StackType_t xMVPTestStack[configMINIMAL_STACK_SIZE];

EMC2305_HandleTypeDef chip;


// CONSTANTS TO CHANGE AS NEEDED
#define PDE_FAN_PWM_TARGET          75      // percentage
#define TEMP_THRESHOLD              40      // degrees celsius
#define TEMP_THRESHOLD_MC          (TEMP_THRESHOLD * 1000) // degrees celsius in millicelsius

#define TEMP_TASK_DELAY      pdMS_TO_TICKS(100)

void PDE_Temp_Task(void *pvParameters) {
    TempMsg_t message;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    static CAN_TxHeaderTypeDef temp_header = {0};
    static uint8_t temp_tx_data[8];

    static CAN_TxHeaderTypeDef fans_header = {0};
    static uint8_t fans_tx_data[8] = {0};

    radiator_fanspeed_t FanMsg = {0};

    if(Cooling_Init() != FAN_CHIP_OK) {
        Error_Handler();
    }

    while (1) {

        // Start ADC reading
        if (Temp_StartADC(true) != TEMP_OK) {
            Error_Handler();
        };
        // Block until we receive data in queue
        if (Temp_GetReading(&message, portMAX_DELAY) == TEMP_OK) {
            printf("ADC: %u Temp: %ld.%03ld C\n\r", message.adc_val, GetTempInt(message.temp_data), GetTempFrac(message.temp_data)); // for debug as needed
            
            PackTempCANMessage(&temp_header, &message, temp_tx_data);
            
            if (can_send(hcan1, &temp_header, temp_tx_data, CAN_TASK_DELAY) != CAN_OK) { // send temp message
                HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
            }
            
            // If temp exceeds threshold, set fan to target PWM
            if (GetTempInt(message.temp_data) >= TEMP_THRESHOLD_MC) {
                EMC2305_SetFanPWM(&chip, EMC2305_FAN1, PDE_FAN_PWM_TARGET); // setting both fans bc I don't remember which one's plugged in
                EMC2305_SetFanPWM(&chip, EMC2305_FAN2, PDE_FAN_PWM_TARGET);
                
                PackFanCANMessage(&fans_header, &FanMsg, fans_tx_data);
                if (can_send(hcan1, &fans_header, fans_tx_data, CAN_TASK_DELAY) != CAN_OK) { // send fan message
                    HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
                }
            }
        }

        HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime,TEMP_TASK_DELAY);
        
    }
}


int main(void) {

    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    if (PumpController_Init() != PUMP_CONTROLLER_OK) Error_Handler();

    // Create tasks
    xTaskCreateStatic(
        Blinky_Task,
        "Blinky",
        configMINIMAL_STACK_SIZE,
        NULL,
        BLINKY_TASK_PRIO,
        xBlinkyStack,
        &xBlinkyTaskBuffer
    );
    xTaskCreateStatic(PDE_Temp_Task,
        "PDE Temp Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TEMP_TASK_PRIO,
        xMVPTestStack,
        &xMVPTestTaskBuffer
    );

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}