/*
 * servo_pwm.c
 *
 * Created: 6/4/2025 11:22:58 PM
 * Author: Stoica
 *
 * This module generates PWM signals for controlling two servo motors
 * on pins PB0 and PB1 using TCB0 timer interrupts every 10 microseconds.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo_pwm.h"

#define SERVO_PERIOD_US     20000        // standard servo refresh rate: 20ms
#define TIMER_STEP_US       10           // timer interrupt resolution: 10us
#define US_TO_TICKS(us)     ((us) / TIMER_STEP_US)

static volatile uint16_t servo_ticks_ch0 = 150; // default pulse width: 150 * 10us = 1.5ms
static volatile uint16_t servo_ticks_ch1 = 150;

static volatile uint16_t tick_counter = 0;

void servo_pwm_init(void) {
    // set PB0 and PB1 as outputs
    PORTB.DIRSET = PIN0_bm | PIN1_bm;

    // configure TCB0 to trigger an interrupt every 10us
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm; // clock = 10 MHz (assuming 20 MHz / 2)
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;                    // periodic interrupt mode
    TCB0.CCMP = 100;                                    // compare value for 10us at 10 MHz
    TCB0.INTCTRL = TCB_CAPT_bm;                         // enable capture/compare interrupt
}

void servo_set_pulse_us(uint8_t channel, uint16_t pulse_us) {
    uint16_t ticks = US_TO_TICKS(pulse_us);

    if (channel == 0) {
        servo_ticks_ch0 = ticks;
    } else if (channel == 1) {
        servo_ticks_ch1 = ticks;
    }
}

ISR(TCB0_INT_vect) {
    if (tick_counter == 0) {
        // start pulses on both channels
        PORTB.OUTSET = PIN0_bm | PIN1_bm;
    }

    tick_counter++;

    if (tick_counter == servo_ticks_ch0) {
        PORTB.OUTCLR = PIN0_bm;
    }

    if (tick_counter == servo_ticks_ch1) {
        PORTB.OUTCLR = PIN1_bm;
    }

    if (tick_counter >= US_TO_TICKS(SERVO_PERIOD_US)) {
        tick_counter = 0;
    }

    // clear interrupt flag
    TCB0.INTFLAGS = TCB_CAPT_bm;
}
