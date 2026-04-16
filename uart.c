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

extern intcurrday;
extern intcurrdow;
extern intcurrhour;
extern intcurrmin;
extern intcurrsec;

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
  currday = UCA1RXBUF;
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
    UCA1TXBUF = intcurrday;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = intcurrdow;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = intcurrhour;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF= intcurrmin;
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = intcurrsec;
  } else {
    //Do nothing
  }
}
