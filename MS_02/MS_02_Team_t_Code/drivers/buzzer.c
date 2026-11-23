#include "buzzer.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#define BUZZER_PIN 21
void buzzer_tone(unsigned frequency, unsigned duration_ms) {
    if (frequency < 100 || frequency > 10000) return;
    unsigned slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, false);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    unsigned top = clock_get_hz(clk_sys) / (64u * frequency) - 1u;
    pwm_set_clkdiv(slice, 64.0f);
    pwm_set_wrap(slice, top);
    pwm_set_counter(slice, 0);
    pwm_set_gpio_level(BUZZER_PIN, (top + 1u) / 2u);
    pwm_set_enabled(slice, true);
    printf("BUZZER GP21: command %u Hz for %u ms; listen for sound.\n", frequency, duration_ms);
    fflush(stdout);
    sleep_ms(duration_ms);
    pwm_set_enabled(slice, false);
    gpio_init(BUZZER_PIN);
    gpio_put(BUZZER_PIN, 0);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}
void buzzer_init(void) {
    gpio_init(BUZZER_PIN);
    gpio_put(BUZZER_PIN, 0);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}
