#include "ControlLoop.h"


static StaticSemaphore_t xBpsMutexBuffer;
static StaticSemaphore_t xPumpMutexBuffer;

SemaphoreHandle_t xBpsMutex = NULL;
SemaphoreHandle_t xPumpMutex = NULL;

static uint8_t g_pump_duty_cycle = 0;
bps_status_msg_t bps_status_global = {0};


void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float setpoint, float limit_min, float limit_max, float dt) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->setpoint = setpoint;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->limit_min = limit_min;
    pid->limit_max = limit_max;
    pid->dt = dt;
}

float PID_Update(PID_Controller *pid, float current_value) {
    float error = current_value - pid->setpoint; // If temp > setpoint, error is positive -> increase cooling
    
    // Proportional term
    float Pout = pid->Kp * error;

    // Integral term with anti-windup (clamping)
    pid->integral += error * pid->dt;
    float Iout = pid->Ki * pid->integral;

    // Derivative term
    float derivative = (error - pid->prev_error) / pid->dt;
    float Dout = pid->Kd * derivative;

    // Total output
    float output = Pout + Iout + Dout;

    // Clamp output to limits
    if (output > pid->limit_max) {
        output = pid->limit_max;
    } else if (output < pid->limit_min) {
        output = pid->limit_min;
    }

    pid->prev_error = error;
    return output;
}

MUTEX_init_status_t MUTEX_Init(void) {
    xBpsMutex = xSemaphoreCreateMutexStatic(&xBpsMutexBuffer);
    xPumpMutex = xSemaphoreCreateMutexStatic(&xPumpMutexBuffer);
    
    if (xBpsMutex == NULL || xPumpMutex == NULL) {
        return MUTEX_INIT_FAIL;
    }
    return MUTEX_INIT_OK;
}

void UpdateGlobalBPS(const bps_status_msg_t *new_data) {
    if (xBpsMutex != NULL && xSemaphoreTake(xBpsMutex, portMAX_DELAY) == pdTRUE) {
        bps_status_global = *new_data;
        xSemaphoreGive(xBpsMutex);
    }
}

void ReadGlobalBPS(bps_status_msg_t *dest) {
    if (xBpsMutex != NULL && xSemaphoreTake(xBpsMutex, portMAX_DELAY) == pdTRUE) {
        *dest = bps_status_global;
        xSemaphoreGive(xBpsMutex);
    }
}

void SetPumpDutyCycle(uint8_t duty) {
    if (xPumpMutex != NULL && xSemaphoreTake(xPumpMutex, portMAX_DELAY) == pdTRUE) {
        g_pump_duty_cycle = duty;
        xSemaphoreGive(xPumpMutex);
    }
}

uint8_t GetPumpDutyCycle(void) {
    uint8_t temp = 0;
    if (xPumpMutex != NULL && xSemaphoreTake(xPumpMutex, portMAX_DELAY) == pdTRUE) {
        temp = g_pump_duty_cycle;
        xSemaphoreGive(xPumpMutex);
    }
    return temp;
}