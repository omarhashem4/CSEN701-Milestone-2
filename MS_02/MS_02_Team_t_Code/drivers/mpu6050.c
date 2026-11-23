#include "mpu6050.h"
#include "pico/stdlib.h"

#define TIMEOUT_US 20000u

static bool read_regs(mpu6050_t *s, uint8_t reg, uint8_t *data, size_t count) {
    if (i2c_write_timeout_us(s->bus, s->address, &reg, 1, true, TIMEOUT_US) != 1)
        return false;
    return i2c_read_timeout_us(s->bus, s->address, data, count, false, TIMEOUT_US)
        == (int)count;
}
static bool write_reg(mpu6050_t *s, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return i2c_write_timeout_us(s->bus, s->address, data, 2, false, TIMEOUT_US) == 2;
}
static int16_t signed_word(const uint8_t *data) {
    uint16_t value = ((uint16_t)data[0] << 8) | data[1];
    int32_t result = value;
    if (value & 0x8000u) result -= 65536;
    return (int16_t)result;
}

bool mpu6050_init(mpu6050_t *s, i2c_inst_t *bus) {
    s->bus = bus;
    bool found = false;
    for (unsigned int addr = 0x68; addr <= 0x69; ++addr) {
        s->address = addr;
        if (read_regs(s, 0x75, &s->identity, 1) && s->identity == 0x68) {
            found = true;
            break;
        }
    }
    if (!found || !write_reg(s, 0x6B, 0x80)) return false;
    sleep_ms(100);
    // Wake, use X gyro PLL clock, enable all accelerometer and gyro axes.
    if (!write_reg(s, 0x6B, 0x01) || !write_reg(s, 0x6C, 0x00)) return false;
    sleep_ms(100);
    if (!write_reg(s, 0x1A, 0x03) || // Digital low-pass filter.
        !write_reg(s, 0x19, 0x09) || // 100 Hz internal sample rate.
        !write_reg(s, 0x1B, 0x00) || // +/-250 degrees/second.
        !write_reg(s, 0x1C, 0x00))   // +/-2 g.
        return false;
    return true;
}

bool mpu6050_read(mpu6050_t *s, mpu6050_sample_t *sample) {
    uint8_t data[14];
    if (!read_regs(s, 0x3B, data, sizeof(data))) return false;
    for (unsigned int i = 0; i < 3; ++i) {
        sample->accel[i] = signed_word(&data[2*i]);
        sample->gyro[i] = signed_word(&data[8+2*i]);
    }
    sample->temperature = signed_word(&data[6]);
    return true;
}
