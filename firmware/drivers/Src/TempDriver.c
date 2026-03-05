#include "pumpController.h"

// extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef* hadc1;
// adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q);
extern const int16_t temp_table[4096];

#define ADC_ITEM_SIZE sizeof(uint16_t)
#define ADC_QUEUE_LENGTH 2

QueueHandle_t adc_queue;
uint8_t adc_qStorage[ADC_QUEUE_LENGTH * ADC_ITEM_SIZE];
static StaticQueue_t xStaticQueue_adc;

ADC_ChannelConfTypeDef ADC_Config = {
    .Channel = TEMP1_ADC_CHANNEL,
    .SamplingTime =  TEMP1_SAMPLE_TIME
};

Temp_Status_t Temp_ADC_Init() {
    /* Initialize queue */

    adc_queue = xQueueCreateStatic(
        ADC_QUEUE_LENGTH, 
        ADC_ITEM_SIZE, 
        adc_qStorage, 
        &xStaticQueue_adc
    );
    
    /* ================ ADC Init Struct ================ */
    ADC_InitTypeDef init = {0};

    init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; /* ADC clock: synchronous */
    init.Resolution = ADC_RESOLUTION_12B;           /* 12 bit ADC */
    init.DataAlign = ADC_DATAALIGN_RIGHT;
    init.ScanConvMode = ADC_SCAN_DISABLE;
    init.EOCSelection = ADC_EOC_SINGLE_CONV;
    init.LowPowerAutoWait = DISABLE;
    init.ContinuousConvMode = DISABLE;              /* Single Conversion */
    init.NbrOfConversion = 1;
    init.DiscontinuousConvMode = DISABLE;
    init.DMAContinuousRequests = DISABLE;
    init.Overrun = ADC_OVR_DATA_OVERWRITTEN;    // Overwrites data on overrun: vs ADC_OVR_DATA_PRESERVED
    init.OversamplingMode = DISABLE;

    /* Software triggered conversion */
    init.ExternalTrigConv = ADC_SOFTWARE_START;
    init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

    /* Initialize ADC */
    volatile adc_status_t s = adc_init(&init, hadc1);
    if (s != ADC_OK) return TEMP_INIT_FAIL;
    
    /* Calibrate after initialization (must be after clock setup)*/

    return TEMP_OK;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if(hadc->Instance==ADC1) {
    /** Initializes the peripherals clock
     */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN5
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = TEMP1_ADC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TEMP1_ADC_PORT, &GPIO_InitStruct);

    /* ADC1 interrupt Init: PRIO MUST BE AT LEAST 5 */
    HAL_NVIC_SetPriority(ADC1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  }
}

Temp_Status_t Temp_StartADC(bool clearQueue) {
    // Clear queue if requested
    if (clearQueue) { xQueueReset(adc_queue); }
    // Start ADC conversion: result will appear in queue
    // if (adc_read(TEMP1_ADC_CHANNEL, TEMP1_SAMPLE_TIME, hadc1, adc_queue) != ADC_OK) {
    if (adc_read(hadc1, &ADC_Config ,adc_queue) != ADC_OK) {
        return TEMP_ADC_START_FAIL;
    }
    return TEMP_OK;
}

Temp_Status_t Temp_GetReading(TempMsg_t *message, TickType_t ticksToWait) {
    // Get ADC value from queue
    if (xQueueReceive(adc_queue, &(message->adc_val), ticksToWait) != pdPASS) { 
        return TEMP_ADC_READ_FAIL;
    }
    message->temp_data = ADCToTemp(message->adc_val);
    return TEMP_OK;
}

int16_t ADCToTemp(uint16_t adc_val) {
    // Convert ADC value to temperature using lookup table
    if (adc_val >= TEMP_TABLE_SIZE) {
        return 0; // Cap at max index
    }
    return temp_table[adc_val];
}