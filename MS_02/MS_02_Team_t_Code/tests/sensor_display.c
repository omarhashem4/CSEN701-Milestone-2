#include <stdio.h>
#include "lcd.h"
#include "mpu6050.h"
#include "ttp223.h"
#include "button.h"

#define BUTTON_PIN 9u  // Standalone button-to-GND test. Disconnect the RGB red lead.
static const unsigned keys[] = {10, 11, 12, 13};

int main(void) {
    stdio_init_all();
    hardware_init();  // LCD on I2C1
    i2c_init(i2c0, 100000);
    for (unsigned pin = 4; pin <= 5; ++pin) {
        gpio_set_function(pin, GPIO_FUNC_I2C);
        gpio_pull_up(pin);
    }
    for (unsigned i = 0; i < 4; ++i) ttp223_init(keys[i]);
    button_init(BUTTON_PIN);
    sleep_ms(2000);
    lcd_init();

    mpu6050_t imu;
    bool ready = false;
    while (true) {
        char line[17];
        snprintf(line, sizeof line, "Keys %d%d%d%d Btn %d",
                 ttp223_read_raw(keys[0]), ttp223_read_raw(keys[1]),
                 ttp223_read_raw(keys[2]), ttp223_read_raw(keys[3]),
                 button_pressed(BUTTON_PIN));
        lcd_send_cmd(0x80);
        lcd_send_string(line);

        if (!ready) ready = mpu6050_init(&imu, i2c0);
        mpu6050_sample_t sample;
        ready = ready && mpu6050_read(&imu, &sample);
        lcd_send_cmd(0xc0);
        if (ready) {
            // All three acceleration axes in hundredths of g, in X/Y/Z order.
            snprintf(line, sizeof line, "%4d %4d %4d  ",
                     sample.accel[0] * 100 / 16384, sample.accel[1] * 100 / 16384,
                     sample.accel[2] * 100 / 16384);
            lcd_send_string(line);
        } else {
            lcd_send_string("MPU unavailable ");
        }
        sleep_ms(150);
    }
}
