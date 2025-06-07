/*
 * attiny804 RC servo control.c
 *
 * Created: 6/4/2025 8:38:27 PM
 * Author : stoica
 *
 *    Practically, I created a module that extends the travel range of servos. 
 *  Normally, an RC command ranges from 1000 µs to 2000 µs, but depending on the 
 *  application I might need more or less. This module allows dynamic correction 
 *  of the servo signal using two potentiometers.
 */

#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pwm_input.h"
#include "adc.h"
#include "servo_pwm.h"

#define LED_PIN PIN0_bm
#define LED_PIN_2 PIN1_bm

 

int main(void) {
    // disable prescaler – use full 20 MHz internal clock
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);
 
    pwm_input_init();     // initialize RC pulse input reading
    adc_init();           // initialize ADC
    servo_pwm_init();     // initialize servo PWM outputs

    sei(); // enable global interrupts

    while (1) {



        // read RC input pulse width for channels 1 and 2
       uint16_t ch1 = pwm_input_get_ch1_us();
       uint16_t ch2 = pwm_input_get_ch2_us();

        // apply logic for servo 1
        if (ch1 >= 850 && ch1 <= 2150) {
            if (ch1 > 1800) {
        // read analog value from potentiometer connected to PA6
      uint16_t  adcValue_1 = adc_read(ADC_MUXPOS_AIN6_gc);				
                // modify max travel based on potentiometer value
				uint16_t pulse1 = SERVO_EXT_MAX_US - (adcValue_1*2);
				if (pulse1<=SERVO_EXT_MIN_US)	{pulse1=SERVO_EXT_MIN_US;}
                servo_set_pulse_us(0, pulse1);		
            } else {
                // send a fixed pulse width when not in the "extended" range
                servo_set_pulse_us(0, SERVO_EXT_MIN_US); // 600 
            }
        }

        // apply logic for servo 2
        if (ch2 >= 850 && ch2 <= 2150) {
            if (ch2 > 1800) {
        // read analog value from potentiometer connected to PA7
      uint16_t  adcValue_2 = adc_read(ADC_MUXPOS_AIN7_gc);        

                // modify max travel based on potentiometer value
                uint16_t pulse2 = SERVO_EXT_MIN_US + (adcValue_2*2);
                if (pulse2>=SERVO_EXT_MAX_US)	{pulse2=SERVO_EXT_MAX_US;}
                servo_set_pulse_us(1, pulse2);
            } else {
                // send a fixed pulse width when not in the "extended" range
                servo_set_pulse_us(1, SERVO_EXT_MAX_US); // 2400
            }
        }
    }
}
