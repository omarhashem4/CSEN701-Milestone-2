#include <stdio.h>
#include "pico/stdlib.h"
#include "buzzer.h"

int main(void) {
    buzzer_init();
    stdio_init_all();
    sleep_ms(3000);
    for (;;) {
        buzzer_tone(1000, 400); sleep_ms(300);
        buzzer_tone(2000, 400); sleep_ms(300);
        buzzer_tone(3000, 400);
        puts("BUZZER GP21: silent for 5 seconds, then repeats. Output commands do not confirm audible sound.");
        fflush(stdout);
        sleep_ms(5000);
    }
}
