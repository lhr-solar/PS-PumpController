#include "pumpController.h"
#include "ADC.h"
#include "tempTable.h"


// adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q);


int getTemp(void) {
    return 1;
    //return adc_to_temp[adc_read(TEMP1_ADC_PORT, TEMP1_ADC_PIN)];
}

void printTemp(void) {
    printf("Current Temperature: %d°C\r\n", getTemp());
}