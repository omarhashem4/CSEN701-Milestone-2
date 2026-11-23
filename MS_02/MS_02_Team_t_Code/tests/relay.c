#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "relay_interface.h"

int main(void) {
    relay_interface_init();
    relay_interface_set(false);
    stdio_init_all();
    bool commanded_on = false;
    bool timed = false;
    absolute_time_t deadline = nil_time;
    uint32_t last_report = to_ms_since_boot(get_absolute_time()) - 1000u;

    while (true) {
        if (!stdio_usb_connected()) {
            relay_interface_set(false);
            commanded_on = false;
            timed = false;
            sleep_ms(10);
            continue;
        }
        int ch = getchar_timeout_us(10000);
        if (ch == 'g' || ch == 'G' || ch == '1') {
            relay_interface_set(true);
            commanded_on = true;
            timed = (ch != '1');
            deadline = make_timeout_time_ms(2000);
            puts(timed ? "RELAY via L293D: ENERGIZE command for 2 seconds; then release."
                       : "RELAY via L293D: ENERGIZE command; send 0 or s to release.");
            fflush(stdout);
        } else if (ch == '0' || ch == 's' || ch == 'S') {
            relay_interface_set(false);
            commanded_on = false;
            timed = false;
            puts("RELAY via L293D: RELEASE command (driver output high-impedance).");
            fflush(stdout);
        }
        if (timed && time_reached(deadline)) {
            relay_interface_set(false);
            commanded_on = false;
            timed = false;
            puts("RELAY via L293D: test complete; RELEASE command.");
            fflush(stdout);
        }
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if ((uint32_t)(now - last_report) >= 1000u) {
            printf("RELAY via L293D GP20: %s | g=2s test 1=energize 0=release; 12V must stay unplugged for bench test.\n",
                   commanded_on ? "ENERGIZE commanded" : "RELEASE commanded");
            fflush(stdout);
            last_report = now;
        }
    }
}
