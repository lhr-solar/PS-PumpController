// note: not fully finalized yet, currently a copy of the working test

#pragma once

#include "pumpController.h"
#include "EMC2305.h"

extern EMC2305_HandleTypeDef chip;
extern I2C_HandleTypeDef hi2c1;

extern StaticTask_t initTaskBuffer;
extern StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

extern StaticTask_t FanControlTaskBuffer;
extern StackType_t FanControlStack[configMINIMAL_STACK_SIZE];

extern StaticTask_t PumpControlTaskBuffer;
extern StackType_t PumpControlStack[configMINIMAL_STACK_SIZE];

extern StaticTask_t FlowrateTaskBuffer;
extern StackType_t FlowrateStack[configMINIMAL_STACK_SIZE];
// QueueHandle_t flowrate_queue;

/**
  * @brief Initializes all EMC2305, then kills itself. Only call from ONE task!
  */
void Init_Task(void* argument);


/**
  * @brief Sets fan speed to 3000 RPM then 8000 RPM on loop, while blinking fanchip LED
  */
void FanControl_Task(void* argument);


/**
  * @brief Sets pump duty cycle to 25% then 100% on loop, while blinking pump LED
  */
void PumpControl_Task(void* argument);

/**
  * @brief Reads flowrate from flowrate driver and prints to UART, while blinking flowrate LED
  */
void Flowrate_Task(void* argument);