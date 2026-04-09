// uart.h
// Written by: Ashley Schultz
// Defines interface to uart
#ifndef UART__H
#define UART__H
#include <stdint.h>

typedef struct {
  uint8_t day;
  uint8_t day_of_week;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
} time_t;

void get_time_from_matlab(time_t *time);

void alarm_trip(time_t *time);

void init_uart(void);

#endif //#ifndef UART__H
