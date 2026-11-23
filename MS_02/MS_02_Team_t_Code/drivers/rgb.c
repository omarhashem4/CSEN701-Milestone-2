#include "rgb.h"
#include <stdio.h>
#include "pico/stdlib.h"
#ifndef COMMON_ANODE
#define COMMON_ANODE 0
#endif
static const unsigned pins[] = {9, 8, 22};
void rgb_colour(int channel) {
    for (int i=0; i<3; ++i) gpio_put(pins[i], (i == channel) ^ COMMON_ANODE);
}
void rgb_init(void) {
    for (int i=0; i<3; ++i) {
        gpio_init(pins[i]);
        gpio_put(pins[i], COMMON_ANODE);
        gpio_set_dir(pins[i], GPIO_OUT);
    }
}
