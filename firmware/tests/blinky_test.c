/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/
/*              BLINKY TEST: Blinks every LED in sequence then loops             */
/*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*.·:·.✧ ✦ ✧.·:·.*/

#include "pumpController.h"


// void LED_Blink_Test() {
//     // Blink all LEDs in sequence
//     for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
//         LED_Blink(led_configs[i]);
//     }
// }

// void LED_On_Test() {
//     // Turn on all LEDs
//     for (int i = 0; i < sizeof(led_configs) / sizeof(GPIO_Pin_t); i++) {
//         LED_On(led_configs[i]);
//     }
// }

int main(){
    HAL_Init();
    SystemClock_Config();
    
    // // Init LEDs
    // if (!LEDs_Init()) Error_Handler();
    // // HAL_GPIO_TogglePin(PUMP_LED_PORT, PUMP_LED_PIN);
    // // HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
    // // HAL_Delay(500);

    // // Init UART
    // if (!mx_uart_init()) Error_Handler();
    // // HAL_GPIO_TogglePin(FLOW_LED_PORT, FLOW_LED_PIN);
    // // HAL_Delay(500);

    // // // Init flowrate
    // if (!MX_TIM2_Init()) Error_Handler();
    // // HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
    // // HAL_Delay(500);
    // // HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);
    

    
    // if (!Temp_ADC_Init()) Error_Handler();
    // // HAL_GPIO_TogglePin(FAN_LED_PORT, FAN_LED_PIN);
    // // HAL_Delay(500);

    // // // Init I2C
    // MX_I2C1_Init();
    // // HAL_GPIO_TogglePin(FANCHIP_LED_PORT, FANCHIP_LED_PIN);
    // // HAL_Delay(500);

    if(!PumpController_Init()) Error_Handler();

    HAL_GPIO_TogglePin(TEMP_LED_PORT, TEMP_LED_PIN);

    while(1){
        
        HAL_GPIO_TogglePin(PUMP_STATUS_LED_PORT, PUMP_STATUS_LED_PIN);
        HAL_Delay(500);
    }

    return 0;
}