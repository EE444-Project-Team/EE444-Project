//            EE444 Project: Motion Sensor Alarm
//                by Ash, Hannah, and Josh




#include <msp430.h>
//#include "transceiver.h"
#include "clocks.h"
#include "uart.h"

extern int IncrementVcore(void);

//global variables
int i;

void main(void) {
  //Setting core voltage to 2
  // for (i = 1; i <= 2; i++) {
  //IncrementVcore();
  //}
  setupClocks();
  init_uart();
  _EINT(); 
  LPM0; 
  //setupTransceiver();
}





/*
//Josh Lab 5
//variables
int i;
int j;
int rcvd;
int send;
int nextChar;

int timeSec;
int timeMin;
char minChar;
char secChar[2];

int numConversion;

int numMsg = 0;
char numMsgChar[3];

int breakpoint;
char tempChar[2];
int tempSum;
int tempAvg;
int curr_temp;
int * volatile ptr30 = (int *) 0x1A1A;
int * volatile ptr85 = (int *) 0x1A1C;
int ref30;
int ref85;
int m;
int b;
int buffer[256];
int * volatile buffPtr = (int *) &buffer[0];
//                        0         1         2         3         4         5     
//                        012345678901234567890123456789012345678901234567890123456
char temperatureMsg[] = {"XXX. The temperature is XX °C. Running time is X:XX\r\n"};
void main(void) {
  //Setting core voltage to 2
    for (i = 1; i <= 2; i++) {
   IncrementVcore();
   }

  //Setting up clocks
    UCSCTL1 = DCORSEL_5; //setting DCORSEL
    UCSCTL2 = 518; //setting FLLN, desired freq = FLLN * 32768hz
    UCSCTL4 = SELA__XT1CLK | SELM__DCOCLK | SELS__DCOCLK; //setting clock sources
    P11SEL = BIT0 | BIT1 | BIT2; //p11.0 ACLK, p11.1 MCLK, p11.2 SMCLK
    P11DIR = BIT0 | BIT1 | BIT2;

  //Setting up timer A
    TA0CTL = TASSEL__ACLK | MC__UP | TACLR;
    TA0CCTL0 = CCIE; //enabling interrupt
    TA0CCR0 = 32768; //this makes it so the timer overflows once per second

  //UART Pins
    P5SEL = BIT6 | BIT7; //P5.7 is UCA1RXD, P5.6 is UCA1TXD
    P5DIR = BIT6; //output

  //Reading calibration values from TLV 
    ref30 = *ptr30;
    ref85 = *ptr85;
    m = (ref85 - ref30)/55; //slope
    b = ref30 - 30*m; //intercept

  //Setting up REF Module
    REFCTL0 = REFVSEL_0 | REFOUT | REFON | REFMSTR; //1.5v, output available

  //Setting up ADC
    ADC12CTL0 = ADC12ON | ADC12SHT03 | ADC12SHT01; //turning on, sample/hold is 512 clk cycles
    ADC12CTL1 = ADC12SHP | ADC12DIV1 | ADC12DIV0 | ADC12SSEL_3; //divide clock by 4, clock is SMCLK, sampcon from sample timer
    ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10; //using temp sensor, vref+
    ADC12IE = ADC12IE0; //enabling interrupt for MEM0
    ADC12CTL0 |= ADC12ENC; //enable conversion

  //Setting up button
    P2SEL &= ~BIT6; //switch i/o
    P2DIR &= ~BIT6; //switch input
    P2IES |= BIT6; //triggers ISR on high to low 
    P2REN |= BIT6; 
    P2OUT |= BIT6; //Pullup resistor
    P2IE  |= BIT6; //enable ISR
    P2SEL &= ~BIT7; //switch i/o
    P2DIR &= ~BIT7; //switch input
    P2IES |= BIT7; //triggers ISR on high to low 
    P2REN |= BIT7; 
    P2OUT |= BIT7; //Pullup resistor
    P2IE  |= BIT7; //enable ISR

  //UART
    UCA1CTL1 |= UCSWRST;
    UCA1CTL0 = UCPEN | UCPAR; //LSB first
    UCA1CTL1 = UCSSEL_2; //SMCLK
    UCA1BR0 = 110;
    UCA1BR1 = 0;
    UCA1MCTL |= UCBRF_10 | UCBRS_6 | UCOS16;
    UCA1IE |= UCTXIE; 
    UCA1CTL1 &= ~UCSWRST;
    UCA1IFG = 0;



 
  _EINT(); 
  LPM0; 
}

void UART_ISR(void) __interrupt [USCI_A1_VECTOR] {
switch(UCA1IV) {
  case 0:break;
  case 2:break;  //RXIFG
  case 0x04: //TXIFG
  if (j < 54) {
  UCA1TXBUF = temperatureMsg[j];
  j++;
  } else {
  break; }
  default: break;
  } }

void Timer_ISR(void) __interrupt [TIMER0_A0_VECTOR] {
  if (timeSec != 59) {
  timeSec++;
  } else {
  timeSec = 0;
  timeMin++;
  TA0CCTL0 &= ~CCIFG;
}
}

void Button_ISR(void) __interrupt [PORT2_VECTOR] {
breakpoint = 1;
  switch(P2IV) {
    case 2: break;
    case 4: break;
    case 6: break;
    case 8: break;
    case 10: break;
    case 12: break;
    case 0x0E:
    case 0x10:
    ADC12CTL0 |= ADC12SC; //starting conversion
    numConversion = 0;
    numMsg++;
    tempSum = 0;
    break;
  }
}

void ADC_ISR(void) __interrupt [ADC12_VECTOR] {
  switch(ADC12IV) {
  case 0: break;
  case 2: break;
  case 4: break;
  case 6:
    if (numConversion < 8) {
    curr_temp = (ADC12MEM0 - b)/m; //reading output from ADC, y = mx + b -> x = (y-b)/m

    if (buffPtr < &buffer[255]) {
    *buffPtr = curr_temp;
    buffPtr += 1;
    } else if (buffPtr == &buffer[255]) {
    *buffPtr = curr_temp;
    buffPtr = &buffer[0];
    }
    
    ADC12CTL0 |= ADC12SC; //starting conversion
    numConversion++;
    tempSum += curr_temp;
    } else {
    tempAvg = tempSum / 8;
  
    //message preparation
    tempChar[0] = '0' + (tempAvg / 10); //prepping temp
    tempChar[1] = '0' + (tempAvg % 10);

    minChar = '0' + (timeMin);          //prepping time
    secChar[0] = '0' + (timeSec / 10);
    secChar[1] = '0' + (timeSec % 10);

    numMsgChar[0] = '0' + (numMsg / 100); //prepping message increment
    numMsgChar[1] = '0' + (numMsg / 10);
    numMsgChar[2] = '0' + (numMsg % 10);

    temperatureMsg[0] = numMsgChar[0]; //putting chars in message
    temperatureMsg[1] = numMsgChar[1];
    temperatureMsg[2] = numMsgChar[2];
    temperatureMsg[24] = tempChar[0];
    temperatureMsg[25] = tempChar[1];
    temperatureMsg[47] = minChar;
    temperatureMsg[49] = secChar[0];
    temperatureMsg[50] = secChar[1];
    UCA1TXBUF = temperatureMsg[0];
    j = 1;
    numConversion = 0;
    ADC12IFG = 0;
    }
    break;
  default: break;
  }
 }



















  /* Josh Lab 3
  #include <msp430.h>

//variables
int curr_temp;
int i;
int breakpoint;
int * volatile ptr30 = (int *) 0x1A1A;
int * volatile ptr85 = (int *) 0x1A1C;
int ref30;
int ref85;
int m;
int b;
int buffer[256];
int * volatile buffPtr = (int *) &buffer[0];


void main(void){

  //Setting core voltage to 2
    for (i = 1; i <= 2; i++) {
   IncrementVcore();
   }

  //Reading calibration values from TLV 
    ref30 = *ptr30;
    ref85 = *ptr85;
    m = (ref85 - ref30)/55; //slope
    b = ref30 - 30*m; //intercept

  //Setting up buffer
    buffPtr = &buffer[0];

  //Setting up clocks
    UCSCTL1 = DCORSEL_5; //setting DCORSEL
    UCSCTL2 = 487; //setting FLLN, desired freq = FLLN * 32768hz
    UCSCTL4 = SELA__XT1CLK | SELM__DCOCLK | SELS__DCOCLK; //setting clock sources
    UCSCTL8 &= ~SMCLKREQEN; //disable clock request for SMCLK
    P11SEL = BIT0 | BIT1 | BIT2; //p11.0 ACLK, p11.1 MCLK, p11.2 SMCLK
    P11DIR = BIT0 | BIT1 | BIT2;


  //Setting up timer A
    TA0CTL = TASSEL__ACLK | MC__UP | TACLR;
    TA0CCTL0 = CCIE; //enabling interrupt
    TA0CCR0 = 32768; //this makes it so the timer overflows once per second
  

  //Setting up LED and I/O pin
    P1DIR = BIT0 | BIT2; //output, p1.0 is LED, p1.2 is i/o pin


  //Setting up button
    P2SEL &= ~BIT6; //switch i/o
    P2DIR &= ~BIT6; //switch input
    P2IES |= BIT6; //triggers ISR on high to low 
    P2REN |= BIT6; 
    P2OUT |= BIT6; //Pullup resistor
    P2IE  |= BIT6; //enable ISR



  //Setting up REF Module
    REFCTL0 = REFVSEL_0 | REFOUT | REFON | REFMSTR; //1.5v, output available


  //Setting up ADC
    ADC12CTL0 = ADC12ON | ADC12SHT03 | ADC12SHT01; //turning on, sample/hold is 512 clk cycles
    ADC12CTL1 = ADC12SHP | ADC12DIV1 | ADC12DIV0 | ADC12SSEL_3; //divide clock by 4, clock is SMCLK, sampcon from sample timer
    ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10; //using temp sensor, vref+
    ADC12IE = ADC12IE0; //enabling interrupt for MEM0
    ADC12CTL0 |= ADC12ENC; //enable conversion

  _EINT(); 
  LPM0; 
}


void Button_ISR(void) __interrupt [PORT2_VECTOR] {
  switch(P2IV) {
    case 0x0E:
    breakpoint = 1; //breakpoint here
    break;
  }
}


void Timer_ISR(void) __interrupt [TIMER0_A0_VECTOR] {
 P1OUT ^= BIT0; //toggle LED
 P1OUT |= BIT2; //setting pin to 1
 ADC12CTL0 |= ADC12SC; //starting conversion
 TA0CCTL0 &= ~CCIFG;
}


void ADC_ISR(void) __interrupt [ADC12_VECTOR] {
  switch(ADC12IV) {
  case 6:
    curr_temp = (ADC12MEM0 - b)/m; //reading output from ADC, y = mx + b -> x = (y-b)/m
    P1OUT &= ~BIT2; //setting pin to 0

    if (buffPtr < &buffer[255]) {
    *buffPtr = curr_temp;
    buffPtr += 1;
    } else if (buffPtr == &buffer[255]) {
    *buffPtr = curr_temp;
    buffPtr = &buffer[0];
    }

    breakpoint = 1;
    break;
  }
}
