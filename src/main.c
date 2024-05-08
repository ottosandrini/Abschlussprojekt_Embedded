#include <stm32f0xx.h>
#include "mci_clock.h"
#include <stdio.h>


// This is a simple macro to print debug messages of DEBUG is defined
#define DEBUG

#ifdef DEBUG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif

// Select the Baudrate for the UART
#define BAUDRATE 9600

// PC10 (USART3_TX) available on CN7 pin 1 to CN3 pin RX
// PC11 (USART3_RX) available on CN7 pin 2 to CN3 pin TX

void initUART();
void delay(uint32_t time);

static uint8_t Clock;

volatile uint8_t Out;

int main(void){
    // Configure the system clock to 48MHz
    EPL_SystemClock_Config();
    initUART();
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Aktivierung des Clocks für GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOC->MODER &= ~GPIO_MODER_MODER13; //Konfiguration des Pins als Input
    //  Activate GPIOA 8,9 & 10 as Output
    GPIOA->MODER |= ((uint32_t) 1<<20);
    GPIOA->MODER |= ((uint32_t) 1<<18);
    GPIOA->MODER |= ((uint32_t) 1<<16); 

    int button;
    uint8_t rxb = 0;

    for(;;){
      // Wait for the data to be received
      while( !( USART2->ISR & USART_ISR_RXNE ) ){
      }

      // Read the data from the RX buffer
      rxb = USART2->RDR;
      
      if (rxb == 49){
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 8;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 25;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 26;
      }
      else if (rxb == 50){
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 24;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 9;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 26;
      }
      else {
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 24;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 25;
          GPIOA->BSRR |= GPIO_BSRR_BS_0 << 10;
      }

      LOG("[DEBUG-LOG]: Setting %d\r\n", rxb );
      
     // check button press
      button = GPIOC->IDR &= ((uint32_t) 1<<13);
      if (button == 0) {
        LOG("Der Knopf wird gedrueckt!! \n");
        continue;
      }

      // State machine
      state_table[curr_state]();
      LOG("[DEBUG-LOG] Time: %d | Out: %d \r\n", Clock, Out );
      delay(1000000);
      Clock++;
    }
}

void stateA(void){
  if( Clock == 2 ) { /* Change State? */
    curr_state = STATE_B; /* Next State */
    Out=1;
  }
}


void stateB(void){
  if( Clock == 5 ) { /* Change State? */
    curr_state = STATE_C; /* Next State */
    Out=2;
  }
}
void stateC(void){
  if( Clock == 9 ) { /* Change State? */
	Clock = 0;
	curr_state = STATE_A; /* Next State */
	Out=0;
  }
}
void initializeSM(void)
{
  curr_state = STATE_A;
  Out=0;
  Clock = 1;
}






void initUART(){
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


/**
 * @brief Delays the program execution for a specified amount of time.
 * @param time The amount of time to delay in number of cycles.
 * @return 0 when the delay is completed.
 */
void delay(uint32_t time){
    for(uint32_t i = 0; i < time; i++ ){
        asm("nop"); // No operation, used for delaying
    }
    return 0;
}



// For supporting printf function we override the _write function to redirect the output to UART
int _write( int handle, char* data, int size ) {
    int count = size;
    while( count-- ) {
        while( !( USART2->ISR & USART_ISR_TXE ) ) {};
        USART2->TDR = *data++;
    }
    return size;
}

