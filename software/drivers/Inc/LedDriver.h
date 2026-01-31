// Driver for Status LEDs on pump controller board

#pragma once

#include "pindef.h"

#define TOGGLE_TIME 500 // same as blinky

void LED_Init(void);
void LED_Toggle(GPIO_Pin_t led_pin);