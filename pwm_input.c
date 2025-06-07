// pwm_input.c
#include "pwm_input.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 20000000UL

static volatile uint16_t ch1_start = 0;
static volatile uint16_t ch2_start = 0;
static volatile uint16_t ch1_pulse = 0;
static volatile uint64_t ch2_pulse = 0;

void pwm_input_init(void) {
	// configureaza PA2 si PA3 ca input
	PORTA.DIRCLR = PIN2_bm | PIN3_bm;

	// enable pullup optional
	// PORTA.PIN2CTRL |= PORT_PULLUPEN_bm;
	// PORTA.PIN3CTRL |= PORT_PULLUPEN_bm;

	// interrupt la orice schimbare de stare
	PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN3CTRL = PORT_ISC_BOTHEDGES_gc;

	// configureaza TCA0 ca timer simplu, tick la 1 us
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc; // prescaler 1
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
	TCA0.SINGLE.CTRLB = 0;
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;

	// enable interrupt pentru PORTA
	VPORTA.INTFLAGS = PIN2_bm | PIN3_bm;
}

ISR(PORTA_PORT_vect) {
	uint16_t now = TCA0.SINGLE.CNT;
	static uint8_t last_pa = 0;
	uint8_t pins = VPORTA.IN;

	// canal 1 - PA2
	if (PORTA.INTFLAGS & PIN2_bm) {
		if ((pins & PIN2_bm) && !(last_pa & PIN2_bm)) {
			ch1_start = now;
			} else if (!(pins & PIN2_bm) && (last_pa & PIN2_bm)) {
			ch1_pulse = now - ch1_start;
		}
		PORTA.INTFLAGS = PIN2_bm;
	}

	// canal 2 - PA3
	if (PORTA.INTFLAGS & PIN3_bm) {
		if ((pins & PIN3_bm) && !(last_pa & PIN3_bm)) {
			ch2_start = now;
			} else if (!(pins & PIN3_bm) && (last_pa & PIN3_bm)) {
			ch2_pulse = now - ch2_start;
		}
		PORTA.INTFLAGS = PIN3_bm;
	}

	last_pa = pins;
}

uint16_t pwm_input_get_ch1_us(void) {
    return ((uint64_t)ch1_pulse * 1000000UL) / 20000000UL;		
}

uint16_t pwm_input_get_ch2_us(void) {
    return ((uint64_t)ch2_pulse * 1000000UL) / 20000000UL;	
}

