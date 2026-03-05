/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */
/*    TEMP TEST: PRINTS MEASURED ADC VALUE AND CONVERTED TEMP ON LOOP + BLINKY     */
/* .·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·. */

#include "pumpController.h"
#include "tasks.h"

StaticTask_t initsTaskBuffer;
StackType_t initsTaskStack[200];
StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[ 200 ];
StaticTask_t xADCTaskBuffer;
StackType_t xADCStack[ 200 ];

// Initialize UART
void Inits_Task(void* argument) {
    // Init UART printf
    husart1->Init.BaudRate = UART_BAUD_RATE;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);

    // Task kills itself
    vTaskDelete(NULL);
}

void ADC_Task(void *pvParameters) {
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

void Task_Blinky(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main() {
    HAL_Init();
    SystemClock_Config();
    
    if(PumpController_Init() == PUMP_CONTROLLER_INIT_FAIL) Error_Handler();

    xTaskCreateStatic(Inits_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initsTaskStack,
        &initsTaskBuffer);
    
    xTaskCreateStatic(
        ADC_Task,
        "ADC Task",
        configMINIMAL_STACK_SIZE,
        (void*) 1,
        ADC_TASK_PRIO,
        xADCStack,
        &xADCTaskBuffer
    );

    xTaskCreateStatic(
        Task_Blinky,
        "Blinky",
        configMINIMAL_STACK_SIZE,
        (void*) 1,
        tskIDLE_PRIORITY+3,
        xBlinkyStack,
        &xBlinkyTaskBuffer
    );

    vTaskStartScheduler();

    return 0;
}