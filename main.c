//            EE444 Project: Motion Sensor Alarm
//                by Ash, Hannah, and Josh

#include <msp430.h>
//#include "transceiver.h"
#include "clocks.h"
#include "uart.h"

extern int IncrementVcore(void);

//global variables
//int i;

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