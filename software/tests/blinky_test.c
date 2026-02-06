/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*              BLINKY TEST: Blinks every LED in sequence then loops             */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#include "stm32xx_hal.h"
#include "pumpController.h"

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