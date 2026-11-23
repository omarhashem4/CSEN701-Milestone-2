#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

// Standalone 50 Hz hobby-servo test; uses only a limited pulse range.
void servo_init(unsigned int pin);
void servo_set_pulse_us(unsigned int pin, uint16_t pulse_us);
void servo_stop(unsigned int pin);
#endif
