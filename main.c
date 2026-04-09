//            EE444 Project: Motion Sensor Alarm
//                by Ash, Hannah, and Josh

#include <msp430.h>
//#include "transceiver.h"
#include "clocks.h"
#include "uart.h"

extern int IncrementVcore(void);
extern int alarm_state = 0;

void main(void) {
  setupClocks();
  init_uart();
  _EINT(); 
  LPM0; 
  //setupTransceiver();
}
