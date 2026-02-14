#pragma once
#include "pumpController.h"

void Error_Handler(void);
void SystemClock_Config(void);
void mx_uart_init(void);
void MX_ADC1_Init(void);

extern ADC_HandleTypeDef hADC1;