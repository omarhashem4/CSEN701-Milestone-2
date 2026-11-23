#include "button.h"
#include "pico/stdlib.h"

void button_init(unsigned pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

bool button_pressed(unsigned pin) {
    return !gpio_get(pin);
}
