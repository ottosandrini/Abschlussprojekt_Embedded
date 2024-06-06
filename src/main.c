#include <stm32f0xx.h>
#include "mci_clock.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "statemachine.h"
#include "UART.h"
//#include "epl_usart.h"

// This is a simple macro to print debug messages of DEBUG is defined
#define DEBUG

#ifdef DEBUG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif

// PC10 (USART3_TX) available on CN7 pin 1 to CN3 pin RX
// PC11 (USART3_RX) available on CN7 pin 2 to CN3 pin TX


State_Type curr_state; /* The "current state" */
State_Type * stateptr = &curr_state;

volatile uint8_t buttonpress = 0;
int playingfield[10*10];
int enemy_field[10*10];
int * pfptr;
int * efptr;
char * stateinfo="11111111111111111111";
char ** si = &stateinfo;
// check if state has changed

State_Type laststate;

//      ------------    MAIN    -------------

int main(void){
    // Configure the system clock to 48MHz
    EPL_SystemClock_Config();
    // Configure UART
    initUART2();
    // Configure playing field
    for(int i=0; i<100; i++){
        playingfield[i]=0;
        enemy_field[i]=0;
    }
    // infinite loop
    for(;;){
        // state function pointer
        state_table[curr_state](stateinfo, playingfield, enemy_field, stateptr);
        // state debug message
        if (laststate != curr_state){
            //LOG("[DEBUG-LOG]: %d\r\n", curr_state);
            laststate = curr_state;
        }
    }
}

// ------------------   END MAIN    -------------

void EXTI4_15_IRQHandler(void) // Interrupt function
{
  if (buttonpress == 0){
     buttonpress=1;
     curr_state=STARTS1;
    }
  if (EXTI->PR & (1 << 13)) {
    // Clear the EXTI status flag.
    // Put a breakpoint here and check the value of the flag.
    EXTI->PR |= (1 << 13); 
  } 
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
    printf("RECEIVED; %s \n", msgdata);
    *msg = msgdata;
}


void USART2_IRQHandler(void){
    //LOG("Received char\n"); // Add debug print
    /*
    if ( USART2->ISR & USART_ISR_RXNE ) {
        uint8_t c = USART2->RDR;
        addchar(c, lastmsg, count, lstateinfo);
    }
    */
    readUART2(si);
}


// To support printf redirect output to UART
int _write( int handle, char* data, int size ) {
    int count = size;
    while( count-- ) {
        while( !( USART2->ISR & USART_ISR_TXE ) ) {};
        USART2->TDR = *data++;
    }
    return size;
}

