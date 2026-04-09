#include <msp430.h>
#include "uart.h"

int size = 0;

extern alarm_state;

void init_uart(void) {
  // Set up UART
  P5SEL |= BIT6 | BIT7;
  UCA1CTL1 |= UCSWRST;
  UCA1CTL0 |= UCPEN | UCPAR;
  UCA1CTL1 |= UCSSEL_2; // UART using SMCLK
  UCA1BR0 = 17;
  UCA1BR1 = 0;
  UCA1MCTL |= UCBRF_0 | UCBRS_3;
  UCA1CTL1 &= ~UCSWRST;

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
  if (alarm_state) {
    UCA1TXBUF = 2;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = time->day;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = time->day_of_week;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = time->hour;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF= time->min;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = time->sec;
  } else {
    //Do nothing
  }
}
