#include "servo.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void servo_init(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    unsigned int slice = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    // One PWM count is one microsecond; 20,000 counts is a 50 Hz period.
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / 1000000.0f);
    pwm_config_set_wrap(&config, 19999);
    pwm_init(slice, &config, false);
    pwm_set_gpio_level(pin, 1500);
    pwm_set_enabled(slice, true);
}

void servo_set_pulse_us(unsigned int pin, uint16_t pulse_us) {
    if (pulse_us < 1300) pulse_us = 1300;
    if (pulse_us > 1700) pulse_us = 1700;
    pwm_set_gpio_level(pin, pulse_us);
}

void servo_stop(unsigned int pin) {
    pwm_set_gpio_level(pin, 0);
}
