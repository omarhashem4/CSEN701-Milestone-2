#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    sleep_ms(2000);
    mpu6050_t sensor;

    while (true) {
        if (!mpu6050_init(&sensor, i2c0)) {
            puts("MPU ERROR: no verified MPU6050 at 0x68/0x69; check 3V3, GND, SDA=GP4, SCL=GP5.");
            fflush(stdout);
            sleep_ms(1000);
            continue;
        }
        printf("MPU FOUND address=0x%02X WHO_AM_I=0x%02X\n",
               (unsigned int)sensor.address, (unsigned int)sensor.identity);
        unsigned int count = 0;
        while (true) {
            mpu6050_sample_t sample;
            if (!mpu6050_read(&sensor, &sample)) {
                puts("MPU ERROR: I2C read failed; retrying detection.");
                fflush(stdout);
                break;
            }
            // Repeat identity so a serial monitor opened later sees it too.
            if (count++ % 20u == 0u)
                printf("MPU OK address=0x%02X WHO_AM_I=0x%02X\n",
                       (unsigned int)sensor.address, (unsigned int)sensor.identity);
            printf("ACC[g] X=%+.3f Y=%+.3f Z=%+.3f | GYRO[deg/s] X=%+.2f Y=%+.2f Z=%+.2f\n",
                   sample.accel[0]/16384.0, sample.accel[1]/16384.0,
                   sample.accel[2]/16384.0, sample.gyro[0]/131.0,
                   sample.gyro[1]/131.0, sample.gyro[2]/131.0);
            fflush(stdout);
            sleep_ms(250);
        }
        sleep_ms(1000);
    }
}
