#include <msp430.h>
#include "uart.h"
#include "transceiver.h"

int size = 0;
extern alarm_state;

uint8_t currday;
uint8_t currdow;
uint8_t currhour;
uint8_t currmin;
uint8_t currsec;


//FIXME delete these
uint8_t testvar;
uint8_t breakpoint;

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


void get_time_from_matlab(void) {
  delay();
  UCA1TXBUF = 1;
  while(!(UCA1IFG&UCRXIFG));
  //testvar = UCA1RXBUF;
  currday = UCA1RXBUF;
  breakpoint = 1;
  while(!(UCA1IFG&UCRXIFG));
  currdow = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  currhour = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  currmin = UCA1RXBUF;
  while(!(UCA1IFG&UCRXIFG));
  currsec = UCA1RXBUF;
}

void alarm_trip(void) {
  if (alarm_state) {
    UCA1TXBUF = 2;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = currday;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = currdow;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = currhour;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF= currmin;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = currsec;
  } else {
    //Do nothing
  }
}
