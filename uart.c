#include <msp430.h>

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
  UCA1IE |= UCRXIE;
}

void uart_ISR(void) __interrupt[USCI_A1_VECTOR] {
  switch(UCA1IV) {
    case 0:
      break;
    case 2:
      while(!(UCA1IFG&UCTXIFG));
      UCA1TXBUF = UCA1RXBUF;
      break;
    case 4:
      break;
    default:
      break;
  }
}