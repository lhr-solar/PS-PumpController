// note: not fully finalized yet, currently a copy of the working test

#pragma once

#include "pumpController.h"

/**
  * @brief Initializes all EMC2305, then kills itself. Only call from ONE task!
  */
void Init_Task(void* argument);


/**
  * @brief Sets fan speed to 3000 RPM then 8000 RPM on loop, while blinking fanchip LED
  */
void FanSpeed_Task(void* argument);


/**
  * @brief Sets pump duty cycle to 25% then 100% on loop, while blinking pump LED
  */
void PumpSpeed_Task(void* argument);

extern EMC2305_HandleTypeDef chip;