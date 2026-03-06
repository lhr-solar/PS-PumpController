#pragma once

#include "pumpController.h"
#include "EMC2305.h"

extern EMC2305_HandleTypeDef chip;
extern I2C_HandleTypeDef hi2c1;

#define INIT_TASK_PRIO       tskIDLE_PRIORITY + 1
extern StaticTask_t initTaskBuffer;
extern StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

#define FAN_TASK_PRIO       tskIDLE_PRIORITY + 4
extern StaticTask_t FanControlTaskBuffer;
extern StackType_t FanControlStack[configMINIMAL_STACK_SIZE];

#define PUMP_TASK_PRIO       tskIDLE_PRIORITY + 5
extern StaticTask_t PumpControlTaskBuffer;
extern StackType_t PumpControlStack[configMINIMAL_STACK_SIZE];

#define FLOWRATE_TASK_PRIO       tskIDLE_PRIORITY + 3
extern StaticTask_t FlowrateTaskBuffer;
extern StackType_t FlowrateStack[configMINIMAL_STACK_SIZE];

#define TEMP_TASK_PRIO       tskIDLE_PRIORITY + 2
extern StaticTask_t xTempTaskBuffer;
extern StackType_t xTempStack[configMINIMAL_STACK_SIZE];

/**
  * @brief Initializes EMC2305 and UART, then kills itself. Only call from ONE task!
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