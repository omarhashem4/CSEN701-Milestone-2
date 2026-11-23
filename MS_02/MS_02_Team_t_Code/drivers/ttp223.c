#include "ttp223.h"
#include "pico/stdlib.h"

void ttp223_init(unsigned int pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_down(pin);
}

bool ttp223_read_raw(unsigned int pin) {
    return gpio_get(pin);
}
