#include "statemachine.h"
#include <stm32f0xx.h>
#include <string.h>
#include <stdio.h>
#include "mci_clock.h"
#include "UART.h"

void init_(char*, int*, int*, State_Type*);
void startS1_(char*, int*, int*, State_Type*);
void startS2_(char*, int*, int*, State_Type*);
void field_(char*, int*, int*, State_Type*);
void play_(char*, int*, int*, State_Type*);
void result_(char*, int*, int*, State_Type*);
void gameend_(char*, int*, int*, State_Type*);
void error_(char*, int*, int*, State_Type*);
void unexpected_(char*, int*, int*, State_Type*);
void mybad_(char*, int*, int*, State_Type*);




void (*state_table[])(char*, int*, int *, State_Type*) = { 
    init_,
    startS1_,
    startS2_,
    field_,
    play_,
    result_,
    gameend_,
    error_,
    unexpected_,
    mybad_
    };  


void init_(char *info, int *pf, int * ef, State_Type * curr_state){
    // Button Interrupt:
    // Enable clock for SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // Enable clock for GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // Set GPIOC pin 13 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODER0 << 2*13);
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR0_1 << 2*13);


    // Configure the external interrupt configuration register, for the selected pin.
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
    // Set the interrupt mask register to enable the interrupt for the selected pin.
    EXTI->IMR |= EXTI_IMR_MR0 << 13;
    // Set the interrupt trigger selection register to select the rising edge trigger for the selected pin.
    EXTI->RTSR |= EXTI_RTSR_TR0 << 13;
    EXTI->FTSR &= ~EXTI_FTSR_TR0 << 13;

    // Enable the interrupt for the selected pin.
    NVIC_SetPriority(EXTI4_15_IRQn, 0x3);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    // printf("INIT \r\n");
    *curr_state = STARTS2; /* Next State */
}
  
void startS2_(char *info, int *pf, int *ef, State_Type * curr_state){ // coming from init
    // State needs to wait for Start message received from Interrupt
    //printf("Test");
    // info should contain the Start message as received by the interrupt
    if(info[0]!='1'){
        // store enemy field from checksum
        printf("SSSSSSSSSSSSTTTTTTTTTTTTTTTTAAAAAAAAAAAAATTTTTTTTTTTTEEEEEEEE"); 
        // generate playingfield

        // send checksum 
        *curr_state = PLAY; /* Next State */
    }


}
  
void startS1_(char *info, int *pf, int *ef, State_Type * curr_state){ // coming from button interrupt
    // sends START message and switches to FIELD
    char startmsg[15] = "START11819602\n";
    printf("%s", startmsg);
    *curr_state = FIELD;
}
void field_(char *info, int *pf, int *ef, State_Type * curr_state){  // coming from StartS1
    // generate playingfield and send checksum

    *curr_state = PLAY;
}

void play_(char *info, int *pf, int *ef, State_Type * curr_state){  // coming from StartS2 or Field
    // State does something here
    //*curr_state = RESULT; /* Next State */
}

void result_(char *info, int *pf, int *ef, State_Type * curr_state){
    // State does something here
    *curr_state = GAMEEND; /* Next State */
}

void gameend_(char *info, int *pf, int *ef, State_Type * curr_state){
    // State does something here
    printf("INterrruptworx");
    *curr_state = ERRORt; /* Next State */
}

void error_(char *info, int *pf, int *ef, State_Type * curr_state){
    // State does something here
    *curr_state = GAMEEND; /* Next State */
}

void unexpected_(char *info, int *pf, int *ef, State_Type * curr_state){
    // State does something here
    *curr_state = ERROR; /* Next State */

}

void mybad_(char *info, int *pf, int *ef, State_Type * curr_state){
    // State does something here
    *curr_state = ERROR; /* Next State */
}

void initializeSM(State_Type * curr_state){
    *curr_state = INIT;
}


