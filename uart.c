#include <msp430.h>
#include "uart.h"

int size = 0;

void init_uart(void) {
  
  // Set up UART
  P5SEL |= BIT6 | BIT7;
  UCA1CTL1 |= UCSWRST;
  UCA1CTL0 |= UCPEN | UCPAR;
  UCA1CTL1 |= UCSSEL_2; // UART using SMCLK
  UCA1BR0 = 65;
  UCA1BR1 = 3;
  UCA1MCTL |= UCBRF_0 | UCBRS_2;
  UCA1CTL1 &= ~UCSWRST;

  //setting up timer a
  TA1CTL =  TASSEL__ACLK | MC__UP |TACLR; // selects ACLK for TimerA 1
  TA1CCTL0 = CCIE;
  TA1CCR0 = 32768;
}


void get_time_from_matlab(time_t *time) {
  UCA1TXBUF = 1;
  while(!(UCA1IFG&UCRXIFG));
  time->day = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  time->day_of_week = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  time->hour = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  time->min = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  time->sec = UCA1RXBUF;
}

void alarm_trip(time_t *time) {
  UCA1TXBUF = 1;
}

int x, y, z = 0;
int i = 0;
uint8_t byte_array[100] = {0};
void timerA_ISR(void) __interrupt[TIMER1_A0_VECTOR] {
  time_t time;
  get_time_from_matlab(&time);
  alarm_trip(&time);
  x += 1;
  y += 1;
  z += 1;
  i = 0;
  byte_array[i] = (uint16_t) x;
  i += 2;
  byte_array[i] = (uint16_t) y;
  i += 2;
  byte_array[i] = (uint16_t) z;
  i = 0;
  size = sizeof(x) * 3;
  UCA1IE |= UCTXIE;
  TA1CCTL0 &= ~(CCIFG); // Clear timer interrupt flag
}

void uart_ISR(void) __interrupt[USCI_A1_VECTOR] {
  switch(UCA1IV) {
    case 0:
      break;
    case 2:
      
      break;
    case 4:
      UCA1TXBUF = byte_array[i];
      i++;
      if( i >= size) {
        UCA1IE &= ~UCTXIE;
      }
      break;
    default:
      break;
  }
}
