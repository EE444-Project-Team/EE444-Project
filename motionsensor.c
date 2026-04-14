#include <msp430f5438a.h>
#include "uart.h"
time_t *time;
void motion_sensor_init(void)
{

//set up motion sensor pins-gpio
P1SEL &= ~BIT6; //pin 1.6 i/o
P1DIR &= ~BIT6; // pin dir input

P1REN |= BIT6; //pull up/down resistor enable
P1OUT |= BIT6; // selecting pull up

P1IE |= BIT6; //interrupt enable
P1IES |= BIT6; // hi-to-lo interrupt edge select

// Testing w/ LED
P1DIR |= BIT0;
P1OUT |= BIT0;

//rtc setup
 get_time_from_matlab(time);
 RTCCTL1 = 0b00100000;
 RTCSEC = time->sec;
 RTCMIN = time->min;
 RTCHOUR = time->hour;
 RTCDOW = time->day_of_week;
 RTCDAY = time->day;
 RTCMON = 0b00000100; //setting to april
 RTCYEARL = 0b11101010; //setting year
 RTCYEARH = 0b00000111;
 RTCCTL1 |= RTCRDY;
 while (!RTCRDY);
}
/*
void motion_ISR(void) __interrupt [PORT1_VECTOR] {
  switch(P1IV) {
  case 0x0E:
  P1OUT ^= BIT0; //led testing
  time->sec = RTCSEC;
  time->min = RTCMIN;
  time->hour = RTCHOUR;
  time->day_of_week = RTCDOW;
  time->day = RTCDAY;
  alarm_trip(time);
  break;
  default: break;
  } */
