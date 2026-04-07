#include <__cross_studio_io.h>
#include <msp430f5438a.h>

int main(void)
{
 int j = 0;
 UCSCTL1 = DCORSEL_4; //setting DCORSEL
    UCSCTL2 = 243; //setting FLLN, desired freq = FLLN * 32768hz
    UCSCTL4 = SELA__XT1CLK | SELM__DCOCLK | SELS__DCOCLK; //setting clock sources
    P11SEL = BIT0 | BIT1 | BIT2; //p11.0 ACLK, p11.1 MCLK, p11.2 SMCLK
    P11DIR = BIT0 | BIT1 | BIT2;

//set up motion sensor pins-gpio
P1SEL &= ~BIT6; //pin 1.6 i/o
P1DIR &= ~BIT6; // pin dir output

P1REN |= BIT6; //pull up/down resistor enable
P1OUT |= BIT6; // selecting pull up

P1IE |= BIT6; //interrupt enable
P1IES |= BIT6; // hi-to-lo interrupt edge select

// Testing w/ LED
P1DIR = P1DIR | BIT0;
P1OUT |= BIT0;

//rtc setup
 RTCCTL1 = 0b00100000;
 RTCSEC = 0b00000001;
// RTCMIN = pull from pc
// RTCHOUR = pull from pc
// RTCDOW = pull from pc (day of week)
// RTCDAY = pull from pc
 RTCMON = 0b00000100; //setting to april
 RTCYEARL = 0b11101010; //setting year
 RTCYEARH = 0b00000111;
 RTCCTL1 |= RTCRDY;
 while (!RTCRDY);
 j = RTCSEC;
_EINT(); //enable interrupt
LPM0;


}

void motion_ISR(void) __interrupt [PORT1_VECTOR] {

P1OUT ^= BIT0; //led testing

}
