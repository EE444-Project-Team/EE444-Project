#include <msp430.h>
void setupClocks(void) {
    UCSCTL1 = DCORSEL_4; //setting DCORSEL
    UCSCTL2 = 243; //setting FLLN, desired freq = FLLN * 32768hz
    UCSCTL4 = SELA__XT1CLK | SELM__DCOCLK | SELS__DCOCLK; //setting clock sources
    P11SEL = BIT0 | BIT1 | BIT2; //p11.0 ACLK, p11.1 MCLK, p11.2 SMCLK
    P11DIR = BIT0 | BIT1 | BIT2;
}