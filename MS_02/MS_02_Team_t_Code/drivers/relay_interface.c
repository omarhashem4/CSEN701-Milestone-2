#include "relay_interface.h"
#include "pico/stdlib.h"

// GP20 drives L293D enable 3,4EN (chip pin9), never the relay IN directly.
// Chip pin10 is grounded; chip pin11 connects to the low-trigger relay IN.
// Enable HIGH sinks IN through the driver. Enable LOW releases IN (high-Z).
#define RELAY_ENABLE 20u

void relay_interface_init(void) {
    gpio_init(RELAY_ENABLE);
    gpio_put(RELAY_ENABLE, false);
    gpio_set_dir(RELAY_ENABLE, GPIO_OUT);
}
void relay_interface_set(bool energized) {
    gpio_put(RELAY_ENABLE, energized);
}
