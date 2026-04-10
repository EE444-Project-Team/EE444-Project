//            EE444 Project: Motion Sensor Alarm
//                by Ash, Hannah, and Josh

#include <msp430.h>
#include "transceiver.h"
#include "clocks.h"
#include "uart.h"

extern int alarm_state = 1;

void main(void) {
  setupClocks();
  init_uart();
  motion_sensor_init();
  setupTransceiver();
  _EINT(); 
  LPM0; 
}