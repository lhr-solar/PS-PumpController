#pragma once
#include <stdint.h>

typedef enum State_e {OFF = 0, ON} State;

typedef struct {
    uint32_t port;
    uint16_t pin;
} GPIO_Pin_t;