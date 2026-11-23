#include <stdio.h>
#include "lcd.h"
int main(void) {
    stdio_init_all();
    hardware_init();
    sleep_ms(1500);
    for (;;) {
        uint8_t value;
        int result = i2c_read_timeout_us(i2c1, LCD_ADDR, &value, 1, false, 10000);
        if (result != 1) {
            puts("Team8 LCD: NO ACK at 0x27; SDA=GP26 SCL=GP27. LCD initialization skipped.");
            sleep_ms(2000);
            continue;
        }
        lcd_init();
        lcd_send_string("embedded");
        puts("Team8 LCD: ACK at 0x27; sent embedded. Check the LCD for visible text.");
        sleep_ms(5000);
    }
}
