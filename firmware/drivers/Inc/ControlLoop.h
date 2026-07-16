#pragma once

#include "pindefs.h"
#include "printf.h"
#include "stm32xx_hal.h"
#include "EMC2305.h"
#include "common.h"
#include "semphr.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float integral;
    float prev_error;
    float limit_min;
    float limit_max;
    float dt; // Sampling time in seconds
} PID_Controller;

// TUNE THESE VALUES
#define KP                      5.0f
#define KI                      0.1f
#define KD                      0.5f
#define PUMP_TEMP_SETPOINT      45.0f
#define PUMP_PWM_MIN            0
#define PUMP_PWM_MAX            100
#define PUMP_LOOP_DELAY_MS      500
#define PUMP_SAMPLING_TIME      (float)PUMP_LOOP_DELAY_MS / 1000.0f

typedef enum MUTEX_init_status {
    MUTEX_INIT_OK,
    MUTEX_INIT_FAIL
} MUTEX_init_status_t;

extern bps_status_msg_t bps_status_global;

void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float setpoint, float limit_min, float limit_max, float dt);

float PID_Update(PID_Controller *pid, float current_value);

// Initialize the mutexes and shared states
MUTEX_init_status_t MUTEX_Init(void);

// Use thread safe global for BPS status
void UpdateGlobalBPS(const bps_status_msg_t *new_data);
void ReadGlobalBPS(bps_status_msg_t *dest);

// Use thread safe global for pump duty cycle
void SetPumpDutyCycle(uint8_t duty);
uint8_t GetPumpDutyCycle(void);