#ifndef MPU6050_H
#define MPU6050_H
#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"

typedef struct {
    i2c_inst_t *bus;
    uint8_t address;
    uint8_t identity;
} mpu6050_t;
typedef struct {
    int16_t accel[3];
    int16_t gyro[3];
    int16_t temperature;
} mpu6050_sample_t;

// Detect at 0x68/0x69, verify WHO_AM_I, reset, and select +/-2g, +/-250deg/s.
bool mpu6050_init(mpu6050_t *sensor, i2c_inst_t *bus);
bool mpu6050_read(mpu6050_t *sensor, mpu6050_sample_t *sample);
#endif
