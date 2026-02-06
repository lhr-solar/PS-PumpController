#include "stm32xx_hal.h"
#include "pumpController.h"

GPIO_Pin_t led_configs[] = {
    {PUMP_LED_PORT, PUMP_LED_PIN},
    {FAN_LED_PORT, FAN_LED_PIN},
    {FANCHIP_LED_PORT, FANCHIP_LED_PIN},
    {FLOW_LED_PORT, FLOW_LED_PIN},
    {TEMP_LED_PORT, TEMP_LED_PIN},
    {STATUS_LED_PORT, STATUS_LED_PIN}
};

int main(){
    HAL_Init();
    LEDs_Init();

    while(1){
        // Blink all LEDs in sequence
        for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
            LED_Blink(led_configs[i]);
        }

    }

    return 0;
}