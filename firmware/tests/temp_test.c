/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*     TEMP TEST: PRINTS TEMPERATURE READINGS TO VERIFY THERMISTOR ACCURACY      */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

// basic test to verify if thermistor reading is correctly converted

#include "pumpController.h"
#include "tasks.h"
#include "stm32xx_hal.h"
#include "printf.h"

#define STACK_SIZE 200

StaticTask_t xTaskBuffer;
StackType_t xStack[ STACK_SIZE ];

#define QUEUE_LENGTH    10
#define ITEM_SIZE       sizeof( uint32_t )

uint8_t qStorage[QUEUE_LENGTH * ITEM_SIZE];
static StaticQueue_t xStaticQueue;

QueueHandle_t xReadings;

static void error_handler(adc_status_t err) {
    while(1) {
        // set bkpt here
        printf("error handler reached\n");
    }
}

static void success_handler(void) {
    // blinky

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
     // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &led_config); // initialize GPIOA with led_config

    while(1){
        LED_Blink(led_configs[TEMP_LED]);
    }
  }

void Temp_Task(void* argument) {
    

    while (1) {
        
        // Blink temp LED
        LED_Blink(led_configs[TEMP_LED]);
    }
}

void TestADC1(void *pvParameters) {
    // Set bkpt in error_handler();
    uint32_t reading = 0;

    // read once
    for (int i = 0; i < 10; i++) {
        #ifdef ADC_SAMPLETIME_3CYCLES
        adc_status_t stat = adc_read(ADC_CHANNEL_1,  ADC_SAMPLETIME_3CYCLES, hadc1, xReadings);
        #else
        adc_status_t stat = adc_read(ADC_CHANNEL_1,  ADC_SAMPLETIME_2CYCLES_5, hadc1, xReadings);
        #endif
        
        if (stat != ADC_OK) {
            error_handler(stat);
        }
    }

    for (int i = 0; i < 10; i++) {
        xQueueReceive(xReadings, &reading, 0);
    }
    
    success_handler();
}

int main() {
    // GPIO Init
    HAL_Init();
    SystemClock_Config();
    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef input =  {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(GPIOA, &input);
    LEDs_Init();
    LED_On(led_configs[STATUS_LED]);

    xReadings = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, qStorage, &xStaticQueue);

    HAL_ADC_MspInit(hadc1);

    // init ADC
    ADC_InitTypeDef adc_init_1 = {0};
    mx_uart_init();
    MX_I2C1_Init();
    LED_On(led_configs[TEMP_LED]);

    // Init UART printf
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);

    // Testing F4 Init
    adc_init_1.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init_1.Resolution = ADC_RESOLUTION_12B;
    adc_init_1.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init_1.EOCSelection = ADC_EOC_SINGLE_CONV;
    adc_init_1.ContinuousConvMode = DISABLE;
    adc_init_1.NbrOfConversion = 1;
    adc_init_1.DiscontinuousConvMode = DISABLE;
    adc_init_1.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init_1.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init_1.DMAContinuousRequests = DISABLE;
    LED_On(led_configs[FLOW_LED]);

    volatile adc_status_t s = adc_init(&adc_init_1, hadc1);
    s+=0;
    if (s != ADC_OK) error_handler(ADC_INIT_FAIL);
    LED_On(led_configs[FANCHIP_LED]);

    xTaskCreateStatic(TestADC1,
                    "ADC Test",
                    configMINIMAL_STACK_SIZE,
                    (void*) 1,
                    tskIDLE_PRIORITY+4,
                    xStack,
                    &xTaskBuffer);

    vTaskStartScheduler();
    LED_On(led_configs[FAN_LED]);

    // should never reach
    HAL_ADC_MspDeInit(hadc1);

    return 0;
}

