# attiny-804-RC-servo-control
📦 Project: RC Servo Travel Extender – ATtiny804
This project implements an RC servo travel extender using an ATtiny804 microcontroller. The goal is to adjust and extend the servo pulse width based on external inputs (e.g., RC channels), allowing for fine-tuned mechanical travel customization via potentiometers.

🎯 Features
Reads standard RC PWM signals on two input channels.

Dynamically adjusts the pulse width sent to the servos based on potentiometer values.

Allows extension of servo travel beyond the default RC range (e.g., up to 2400 µs).

Protects servo outputs by clamping signals to valid ranges (e.g., 600–2400 µs).

Written in bare-metal C, tested on ATtiny804 @ 20 MHz.

🛠️ Hardware Requirements
ATtiny804 microcontroller

2 potentiometers (connected to analog pins)

2 RC PWM input signals (e.g., from a receiver)

2 servos connected to PWM outputs

📁 Code Structure
main.c – main application loop and initialization

pwm_input.c/.h – RC signal capture via pin-change and software timing

servo_pwm.c/.h – PWM signal generation for servo control

adc.c/.h – reading potentiometer values via ADC

🔧 Adjustments
The adjustment logic ensures:

One servo increases its range from 1500 µs up to 2400 µs.

The second servo reduces its range from 1500 µs down to 600 µs.

Travel extension is scaled based on potentiometer input (0–1023 ADC range).
