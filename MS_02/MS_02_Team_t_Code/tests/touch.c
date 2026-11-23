#include <stdio.h>
#include "pico/stdlib.h"
#include "ttp223.h"

// S1-S4 connect to GP10-GP13, respectively. All modules share 3V3 and GND.
static const unsigned int touch_pins[] = {10u, 11u, 12u, 13u};

int main(void) {
    stdio_init_all();
    for (unsigned int i = 0; i < 4u; ++i) {
        ttp223_init(touch_pins[i]);
    }
    // Leave the touch pads untouched during power-up calibration.
    sleep_ms(2000);

    unsigned int previous = 16u; // Outside the possible four-bit input range.
    uint32_t last_report = to_ms_since_boot(get_absolute_time());
    while (true) {
        unsigned int mask = 0;
        for (unsigned int i = 0; i < 4u; ++i) {
            if (ttp223_read_raw(touch_pins[i])) {
                mask |= 1u << i;
            }
        }
        uint32_t now = to_ms_since_boot(get_absolute_time());
        // Report every change and a one-second heartbeat. Values are raw levels.
        if (mask != previous || (uint32_t)(now - last_report) >= 1000u) {
            printf("FOUR_TOUCH S1(GP10)=%u S2(GP11)=%u S3(GP12)=%u S4(GP13)=%u\n",
                   (mask >> 0) & 1u, (mask >> 1) & 1u,
                   (mask >> 2) & 1u, (mask >> 3) & 1u);
            fflush(stdout);
            previous = mask;
            last_report = now;
        }
        sleep_ms(20);
    }
}
