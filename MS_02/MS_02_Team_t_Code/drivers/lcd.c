#include "lcd.h"
#include <string.h>

#define I2C_PORT i2c1   // SDA=GPIO26, SCL=GPIO27 → I2C1

// Low-level write of one byte to the PCF8574
static void lcd_write(uint8_t data) {
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data, 1, false);
}

static void lcd_toggle_enable(uint8_t val) {
    sleep_us(600);
    lcd_write(val | 0x04);      // EN = 1
    sleep_us(600);
    lcd_write(val & ~0x04);     // EN = 0
    sleep_us(600);
}

// Public: send command
void lcd_send_cmd(uint8_t cmd) {
    uint8_t upper = cmd & 0xF0;
    uint8_t lower = (cmd << 4) & 0xF0;

    // RS=0 (command), backlight=1 (0x08)
    lcd_write(upper | 0x08);
    lcd_toggle_enable(upper | 0x08);

    lcd_write(lower | 0x08);
    lcd_toggle_enable(lower | 0x08);
}

// Public: send one data byte (character)
void lcd_send_data(uint8_t data) {
    uint8_t upper = data & 0xF0;
    uint8_t lower = (data << 4) & 0xF0;

    // RS=1 (data), backlight=1 → 0x09
    lcd_write(upper | 0x09);
    lcd_toggle_enable(upper | 0x09);

    lcd_write(lower | 0x09);
    lcd_toggle_enable(lower | 0x09);
}

// Public: send a whole string
void lcd_send_string(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

// I2C + GPIO init for this LCD
void hardware_init(void) {
    // stdio_init_all() stays in main.c, NOT here

    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz I2C

    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

// Public LCD init – call AFTER hardware_init()
void lcd_init(void) {
    sleep_ms(50);

    // Reset requires single nibbles before 4-bit byte transfers are valid.
    lcd_write(0x38); lcd_toggle_enable(0x38); sleep_ms(5);
    lcd_write(0x38); lcd_toggle_enable(0x38); sleep_ms(1);
    lcd_write(0x38); lcd_toggle_enable(0x38); sleep_ms(1);
    lcd_write(0x28); lcd_toggle_enable(0x28); sleep_ms(1);

    lcd_send_cmd(0x28);   // 4-bit, 2-line, 5x8 font
    lcd_send_cmd(0x08);   // display off
    lcd_send_cmd(0x01);   // clear display
    sleep_ms(2);
    lcd_send_cmd(0x06);   // entry mode
    lcd_send_cmd(0x0C);   // display on, cursor off
}
