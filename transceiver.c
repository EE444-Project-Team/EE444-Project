#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "motionsensor.h"
#include "transceiver.h"

extern alarm_state;
extern time_t * time;

extern currday;
extern currdow;
extern currhour;
extern currmin;
extern currsec;

int intcurrday;
int intcurrdow;
int intcurrhour;
int intcurrmin;
int intcurrsec;

//FIXME delete these
int testvar1;

uint8_t data_array[4];
void setupTransceiver(void) {
  //CE pin on transceiver 
  P1DIR |= BIT4;
  P1OUT &= ~BIT4;
  //IRQ pin transceiver
  P1DIR &= ~BIT2; // input 
  P1IES |= BIT2; //high-to-low
  P1IE |= BIT2;
  //CSN pin transceiver 
  P1DIR |= BIT5;
  P1OUT |= BIT5; //setting CSN
  //SIMO, SOMI, clk
  P3SEL |= BIT1 | BIT2 | BIT3;
  P3DIR |= BIT1 | BIT3;
  P3DIR &= ~BIT2;

//SPI interface setup
  UCB0CTL1 |= UCSWRST;
  UCB0CTL0 = UCMST | UCSYNC | UCMSB | UCCKPH;
  UCB0CTL1 |= UCSSEL__SMCLK;
  UCB0CTL1 &= ~UCSWRST;
  

//Configure transceiver
  sendCmd(0x20, 0x39); //config register 0
  sendCmd(0x00, 0xFF); 
  sendCmd(0x21, 0x00); //disable autoack
  sendCmd(0x24, 0x00); //disable retransmit
  sendCmd(0x23, 0x03); //Set address width to 5bytes (default, not really needed)
  sendCmd(0x26, 0x07); //Air data rate 1Mbit, 0dBm, Setup LNA
  sendCmd(0x31, 0x04); //4 byte receive payload
  sendCmd(0x25, 0x02); //RF Channel 2 (default, not really needed)
  data_array[0] = 0xE7;
  data_array[1] = 0xE7;
  data_array[2] = 0xE7;
  data_array[3] = 0xE7;
  sendPayload(0x2A); //Set RX pipe 0 address
  sendCmd(0x20, 0x3B); //RX interrupt, power up, be a receiver
  powerupDelay();
  P1OUT |= BIT4; //ce high
  P1OUT &= ~BIT5; //reset CSN
  sendByte(0xE2); //Flush RX FIFO 
  P1OUT |= BIT5; //set CSN
}

void Received_Data_ISR(void) __interrupt [PORT1_VECTOR] {
  uint8_t rx_array[4];
  switch (P1IV) {
    case 0x06: //rx
      P1OUT &= ~BIT4; //ce low
      P1OUT &= ~BIT5; //reset CSN
      while (!(UCB0IFG & UCTXIFG));
      UCB0TXBUF = 0b01100001; //read rx cmd
      delay();
      while (!(UCB0IFG & UCTXIFG));
      UCB0TXBUF = 0xFF; 
      delay();
      rx_array[0] = UCB0RXBUF;
      while (!(UCB0IFG & UCTXIFG));
      UCB0TXBUF = 0xFF; 
      delay();
      rx_array[1] = UCB0RXBUF;
      while (!(UCB0IFG & UCTXIFG));
      UCB0TXBUF = 0xFF; 
      delay();
      rx_array[2] = UCB0RXBUF;
      while (!(UCB0IFG & UCTXIFG));
      UCB0TXBUF = 0xFF; 
      delay();
      rx_array[3] = UCB0RXBUF;
      P1OUT |= BIT5; //set CSN

      P1OUT &= ~BIT5; //reset CSN
      sendByte(0xE2); //Flush RX FIFO 
      P1OUT |= BIT5; //set CSN

      P1OUT &= ~BIT5; //reset CSN
      sendCmd(0x27, 0x40); //Clear RF FIFO interrupt
      P1OUT |= BIT5; //set CSN
      switch(rx_array[0]) {
        case 0x17: break; //left
        case 0x1E:
          alarm_state = 0;
        break; //bottom
        case 0x1B: break; //right
        case 0x1D: //setAlarm();
          alarm_state = 1;
        break; //top
        case 0x0F: break; //center
        default: break; //none
      }
      delay();
      P1OUT |= BIT4; //ce high
      break;
    case 0x04: break; //tx
    case 0x0E:
      P1OUT ^= BIT0; //led testing
      
      intcurrsec = RTCSEC;
      intcurrmin = RTCMIN;
      intcurrhour = RTCHOUR;
      intcurrdow = RTCDOW;
      intcurrday = RTCDAY;

      alarm_trip();
      break;
    default: break;
  }
}

uint8_t sendCmd(uint8_t cmd, uint8_t data) {
  uint8_t result = 0;
  P1OUT &= ~BIT5; //reset CSN
  while (!(UCB0IFG & UCTXIFG));
  UCB0TXBUF = cmd; //specify which command
  delay();
  while (!(UCB0IFG & UCTXIFG));
  UCB0TXBUF = data; //send data
  delay();
  result = UCB0RXBUF;
  P1OUT |= BIT5; //set CSN
  return result;
}

uint8_t sendByte(uint8_t cmd) {
  uint8_t result = 0;
  P1OUT &= ~BIT5; //reset CSN
  while (!(UCB0IFG & UCTXIFG));
  UCB0TXBUF = cmd; //specify which command
  delay();
  result = UCB0RXBUF;
  P1OUT |= BIT5; //set CSN
  return result;
}

uint8_t sendPayload(uint8_t cmd) {
  uint8_t x;
  uint8_t result = 0;
  P1OUT &= ~BIT5; //reset CSN
  while (!(UCB0IFG & UCTXIFG));
  UCB0TXBUF = cmd; 
  delay();
  for(x; x<4; x++) {
  while (!(UCB0IFG & UCTXIFG));
  UCB0TXBUF = data_array[x]; 
  delay();
  }

  P1OUT |= BIT5; //set CSN
  return result;
}

void delay(void){ 
  int x = 0;
  for (x; x < 2000 ; x++);
}

void powerupDelay(void) {
  int x = 0;
  for (x; x<18000; x++);
}

  //18000 power up delay
  //2000 delay time
