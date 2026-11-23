#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Your LCD pins
#define I2C_SDA_PIN 26
#define I2C_SCL_PIN 27

// Typical I2C address for PCF8574 backpack
#define LCD_ADDR 0x27

// Function prototypes
void hardware_init(void);
void lcd_init(void);
void lcd_send_string(const char *str);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);

#endif
