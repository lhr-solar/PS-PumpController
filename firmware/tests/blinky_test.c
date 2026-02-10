/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*              BLINKY TEST: Blinks every LED in sequence then loops             */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#include "stm32xx_hal.h"
#include "pumpController.h"

void LED_Blink_Test() {
    // Blink all LEDs in sequence
    for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
        LED_Blink(led_configs[i]);
    }
}

void LED_On_Test() {
    // Turn on all LEDs
    for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
        LED_On(led_configs[i]);
    }
}

int main(){
    HAL_Init();
    LEDs_Init();

    while(1){
        
        LED_Blink_Test();
    }

    return 0;
}