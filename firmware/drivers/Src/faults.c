// #include "faults.h"

// // Event group handle to store fault state bits
// EventGroupHandle_t faultStateBits;

// // Static buffer to store the event handle
// StaticEventGroup_t faultStateBitsBuffer;

// uint8_t faultBits_init(void){
//     faultStateBits = xEventGroupCreateStatic( &faultStateBitsBuffer );
//     if(faultStateBits == NULL){
//         return 0;
//     }
//     return 1;
// }

// void set_faultBit(fault_bit_t bit){
//     // not a valid fault
//     if(bit >= NUM_FAULTS){ 
//         return;
//     }

//     xEventGroupSetBits(faultStateBits, FAULT_BIT(bit));
//     taskYIELD();
// }

// EventBits_t faultBit_wait(fault_bit_t bit, TickType_t xTicksToWait){

//     // NUM_FAULTS indiciates you want to wait for all bits
//     if(bit > NUM_FAULTS){
//         return 0;
//     }

//     // if NUM
//     EventBits_t uxBitsToWaitFor = bit == NUM_FAULTS ? ALL_FAULT_BITS : (FAULT_BIT(bit));

//     EventBits_t pending = xEventGroupWaitBits(
//         faultStateBits,
//         uxBitsToWaitFor,  // wait for any defined fault
//         pdFALSE,          // fault bits are not reset
//         pdFALSE,          // wait for ANY bit to be set
//         xTicksToWait 
//     );
//     return pending;
// }