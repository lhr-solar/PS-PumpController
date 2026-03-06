/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*              BLINKY TEST: Blinks every LED in sequence then loops             */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#include "pumpController.h"

int main(){
    HAL_Init();
    SystemClock_Config();

    if(!PumpController_Init()) Error_Handler();

    HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);

    while(1){
        
        HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);
        HAL_Delay(500);
    }

    return 0;
}