#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "l293d.h"

static bool reverse_pause(void) {
    absolute_time_t deadline = make_timeout_time_ms(2000);
    while (!time_reached(deadline)) {
        if (!stdio_usb_connected()) return false;
        int ch = getchar_timeout_us(0);
        if (ch == 's' || ch == 'S') return false;
        sleep_ms(5);
    }
    return true;
}

int main(void) {
    l293d_init();
    l293d_brake();
    stdio_init_all();
    int direction = 0; // 0 = brake, 1 = forward, -1 = reverse.
    uint32_t last_report = to_ms_since_boot(get_absolute_time()) - 1000u;

    while (true) {
        // Closing/disconnecting the serial monitor cancels the run command.
        if (!stdio_usb_connected()) {
            l293d_brake();
            direction = 0;
            sleep_ms(10);
            continue;
        }
        int ch = getchar_timeout_us(10000);
        if (ch == 's' || ch == 'S') {
            l293d_brake();
            direction = 0;
            puts("L293D: STOP / BRAKE.");
            fflush(stdout);
        } else if (ch == 'g' || ch == 'G' || ch == 'r' || ch == 'R') {
            int requested = (ch == 'r' || ch == 'R') ? -1 : 1;
            if (direction != 0 && direction != requested) {
                l293d_brake();
                direction = 0;
                puts("L293D: BRAKE 2 seconds before reversal; s cancels.");
                fflush(stdout);
                if (!reverse_pause()) continue;
            }
            l293d_drive(requested < 0);
            direction = requested;
            puts(direction > 0 ? "L293D: FORWARD command until s."
                               : "L293D: REVERSE command until s.");
            fflush(stdout);
        }

        uint32_t now = to_ms_since_boot(get_absolute_time());
        if ((uint32_t)(now - last_report) >= 1000u) {
            printf("L293D GP16/GP17: %s | g=forward r=reverse s=stop (commands, no motion feedback)\n",
                   direction == 0 ? "BRAKE" : direction > 0 ? "FORWARD" : "REVERSE");
            fflush(stdout);
            last_report = now;
        }
    }
}
