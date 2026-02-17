#include "pumpController.h"
#include "ADC.h"
#include "tempTable.h"

ADC_HandleTypeDef hADC1;
// adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q);


int getTemp(void) {
    return 1;
    //return adc_to_temp[adc_read(TEMP1_ADC_PORT, TEMP1_ADC_PIN)];
}

void printTemp(void) {
    printf("Current Temperature: %d°C\r\n", getTemp());
}

void MX_ADC1_Init(void) {

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hADC1.Instance = ADC1;
  hADC1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hADC1.Init.Resolution = ADC_RESOLUTION_12B;
  hADC1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hADC1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hADC1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hADC1.Init.LowPowerAutoWait = DISABLE;
  hADC1.Init.ContinuousConvMode = DISABLE;
  hADC1.Init.NbrOfConversion = 1;
  hADC1.Init.DiscontinuousConvMode = DISABLE;
  hADC1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hADC1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hADC1.Init.DMAContinuousRequests = DISABLE;
  hADC1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hADC1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hADC1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hADC1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}