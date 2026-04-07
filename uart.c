#include <msp430.h>
#include <stdint.h>

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

int x, y, z = 0;
int i = 0;
uint8_t byte_array[sizeof(x) * 3] = {0};
void timerA_ISR(void) __interrupt[TIMER1_A0_VECTOR] {
  x += 1;
  y += 1;
  z += 1;
  i = 0;
  byte_array[i] = x;
  i += 2;
  byte_array[i] = y;
  i += 2;
  byte_array[i] = z;
  i = 0;
  UCA1IE |= UCTXIE;
  TA1CCTL0 &= ~(CCIFG); // Clear timer interrupt flag
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
      UCA1TXBUF = byte_array[i];
      i++;
      if( i >= sizeof(byte_array)) {
        UCA1IE &= ~UCTXIE;
      }
      break;
    default:
      break;
  }
}
