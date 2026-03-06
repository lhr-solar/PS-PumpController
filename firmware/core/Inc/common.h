#pragma once
#include "stm32xx_hal.h"
#include "UART.h"
#include "printf.h"
#include "pindefs.h"

void Error_Handler(void);
void SystemClock_Config(void);
bool mx_uart_init(void);