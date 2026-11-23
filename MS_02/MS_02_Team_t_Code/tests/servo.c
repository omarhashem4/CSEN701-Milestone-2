#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"

#ifndef SERVO_PIN
#define SERVO_PIN 6u
#endif

int main(void) {
    stdio_init_all();
    // Give the user time after flashing. Keep signal low until the test starts.
    gpio_init(SERVO_PIN);
    gpio_set_dir(SERVO_PIN, GPIO_OUT);
    gpio_put(SERVO_PIN, false);
    sleep_ms(3000);
    servo_init(SERVO_PIN);

    while (true) {
        servo_set_pulse_us(SERVO_PIN, 1500);
        printf("Servo GP%u: centre, 1500 us at 50 Hz.\n", (unsigned)SERVO_PIN);
        fflush(stdout);
        sleep_ms(2000);

        for (unsigned int cycle = 1; cycle <= 3; ++cycle) {
            const uint16_t positions[] = {1300, 1500, 1700, 1500};
            for (unsigned int step = 0; step < 4; ++step) {
                servo_set_pulse_us(SERVO_PIN, positions[step]);
                printf("Servo: cycle=%u pulse=%uus (command only; no position feedback)\n",
                       cycle, (unsigned int)positions[step]);
                fflush(stdout);
                sleep_ms(1000);
            }
        }
        servo_stop(SERVO_PIN);
        puts("Servo: pulses OFF for 5 seconds, then test repeats.");
        fflush(stdout);
        sleep_ms(5000);
    }
}
