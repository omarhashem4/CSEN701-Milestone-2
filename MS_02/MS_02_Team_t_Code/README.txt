CSEN701 Milestone 2 | Project 5 | The Musical Orientation

Open this folder as a Pico SDK CMake project for the original Raspberry Pi
Pico (RP2040). Configure and build with the Raspberry Pi Pico extension.
Each target builds a separate UF2. Flash one target at a time using BOOTSEL.
For serial output or commands, open the Pico USB serial monitor at 115200.

Target          Connections and observable test
touch           Touch S1-S4 in order on GP10-GP13. Watch the four serial bits.
motion          Tilt the MPU-6050 on GP4 SDA / GP5 SCL. Watch acceleration
                in g and angular velocity in degrees per second over USB.
lcd             GP26 SDA / GP27 SCL, I2C1, address 0x27. Displays "embedded".
sensor_display  Uses the touch, motion and LCD pins above. The first LCD row
                shows four touch bits and an active-low button on GP9. The
                second shows X, Y, Z acceleration in hundredths of g. At rest,
                an axis aligned with gravity should read about +100 or -100.
servo           Cycles the GP6 servo through 1300, 1500 and 1700 us pulses.
servo_gp7       Same test on GP7 for the second servo.
servo_gp8       Same test on GP8 for the third servo.
motor           L293D on GP16 / GP17. Send g for forward, r for reverse,
                s to brake. A direction change includes a two-second pause.
relay           Lock relay interface on GP20 through the L293D. Send g for
                a two-second relay bench test or 0 to release. Follow the
                relay wiring notes before using this target.
buzzer          GP21 cycles through 1, 2 and 3 kHz with silent intervals.
rgb_cathode     Red GP9, green GP8, blue GP22. Common to GND.
rgb_anode       Same sequence and pins. Common to 3V3.

Use a series resistor on each RGB channel. GP8 is shared by the third servo
and green LED across tests. GP9 is shared by the button and red LED. Power
down and disconnect the other component from each shared pin before a test.
Keep Pico I2C signals at 3.3 V. A backpack with 5 V pull-ups needs an I2C
level shifter. The relay and motor tests use different L293D wiring. Their
bench configurations are described in the wiring files alongside this guide.
