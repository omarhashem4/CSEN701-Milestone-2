#include <stdio.h>
#include "pico/stdlib.h"
#include "rgb.h"

int main(void) {
    rgb_init();
    stdio_init_all();
    sleep_ms(3000);
    for (;;) {
        rgb_colour(0); puts("RGB: RED command GP9"); fflush(stdout); sleep_ms(1000);
        rgb_colour(1); puts("RGB: GREEN command GP8"); fflush(stdout); sleep_ms(1000);
        rgb_colour(2); puts("RGB: BLUE command GP22"); fflush(stdout); sleep_ms(1000);
        rgb_colour(-1); puts("RGB: ALL OFF command"); fflush(stdout); sleep_ms(1000);
    }
}
