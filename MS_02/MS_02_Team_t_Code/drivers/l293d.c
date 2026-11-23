#include "l293d.h"
#include "pico/stdlib.h"

// L293D chip pin 1 (enable) is tied HIGH to USB VBUS, per supplied wiring.
// GP16 connects to chip pin 2; GP17 connects to chip pin 7.
#define IN1 16u
#define IN2 17u

void l293d_init(void) {
    gpio_init(IN1);
    gpio_init(IN2);
    gpio_put(IN1, false);
    gpio_put(IN2, false);
    gpio_set_dir(IN1, GPIO_OUT);
    gpio_set_dir(IN2, GPIO_OUT);
}

void l293d_brake(void) {
    // With enable HIGH, both inputs LOW brake the motor rather than coasting.
    gpio_put(IN1, false);
    gpio_put(IN2, false);
}

void l293d_drive(bool reverse) {
    l293d_brake();
    gpio_put(IN1, !reverse);
    gpio_put(IN2, reverse);
}
