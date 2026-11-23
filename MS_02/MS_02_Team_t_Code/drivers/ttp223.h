#ifndef TTP223_H
#define TTP223_H

#include <stdbool.h>

// Initialize a digital TTP223 output as a Pico input.
void ttp223_init(unsigned int pin);
bool ttp223_read_raw(unsigned int pin);

#endif
