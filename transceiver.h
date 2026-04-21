#ifndef  TRANSCEIVER__H
#define  TRANSCEIVER__H
#include <stdint.h>
void setupTransceiver(void);
void Received_Data_ISR(void);
uint8_t sendCmd(uint8_t cmd, uint8_t data);
uint8_t sendByte(uint8_t cmd);
uint8_t sendPayload(uint8_t cmd);
void delay(void);
void powerupDelay(void);

#endif // TRANSCEIVER__H
