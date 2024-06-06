#include "UART.h"
#include <stdlib.h>
#include <stm32f0xx.h>
#include <string.h>
#include "mci_clock.h"

#define BAUDRATE 9600



void initUART2(){

    // Enable peripheral  GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable peripheral  USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 as USART2_TX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] |= 0b0001 << (4*2);

    // Configure PA3 as USART2_RX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= 0b0001 << (4*3);

    // Configure the UART Baude rate Register
    USART2->BRR = (APB_FREQ / BAUDRATE);
    // Enable the UART using the CR1 register
    //USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );
    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | USART_CR1_RXNEIE);
    //EXTI-Interrupt configuration
    //EXTI->IMR |= ((uint32_t) 1<<26);
    //EXTI->EMR |= ((uint32_t) 1<<26);

    // Interrupt things
    // Use 4 bits for 'priority' and 0 bits for 'subpriority'.
    //NVIC_SetPriorityGrouping( 0 );
    // UART receive interrupts should be high priority.
    //uint32_t uart_pri_encoding = NVIC_EncodePriority( 0, 1, 0 );
    //NVIC_SetPriority( USART2_IRQn, uart_pri_encoding );
    NVIC_EnableIRQ( USART2_IRQn );
}
/*
void addchar(uint8_t c,char* lastmsg, int *count, char* stateinfo){
    //printf("%s \n", c);
    if (c == '\n'){
        lastmsg[*count] = 0;
        for(int o=0;o<22;o++){
            stateinfo[o] = lastmsg[o];
        }
        // for loop is trying to do this: stateinfo = lastmsg;
        *count = 0;  
        return;
    }   
    lastmsg[*count] = c;
    *count++;
}
*/

/*void initUART3(){ // EXAKTE KOPIE VON UART2      ------ UMBAUEN!!! ------
     // Enable peripheral  GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable peripheral  USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 as USART2_TX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] |= 0b0001 << (4*2);


    // Configure PA3 as USART2_RX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= 0b0001 << (4*3);

    // Configure the UART Baude rate Register
    USART2->BRR = (APB_FREQ / BAUDRATE);
    // Enable the UART using the CR1 register
    USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );
}

void readUART2(char ** msg){
    // Receives one character at a time
    int count = 0;
    char endchar = '\n';
    char * msgdata = (char *) malloc(sizeof(char));
    while (1){
        // wait for UART Data
        while(!(USART2->ISR & USART_ISR_RXNE)){
        }
        // Read the data from the RX buffer
        msgdata = (char *) realloc(msgdata, count+1+sizeof(char));
        msgdata[count] = USART2->RDR;
        // check if
        if(msgdata[count]==endchar){break;}
        if (count == 30) { // Prevent overflow
            break;
        }
        count++;
    }
    msgdata[count]='\0'; // finish msg string with \0
    *msg = msgdata;
}


void USART2_IRQHandler( void ){
    if ( USART2->ISR & USART_ISR_RXNE ) {
        uint8_t c = USART2->RDR;
        addchar(c, lastmsg, count, lstateinfo);
    }
    readUART2(&lastmsg);
}
*/
