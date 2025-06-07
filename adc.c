/*
 * adc.c
 *
 * Created: 06/04/2024 22:28:06
 * Author: Stoica
 *
 * This module initializes and reads values from ADC0 using 10-bit resolution.
 * It assumes the use of PA6 and PA7 for analog input.
 */

#include "adc.h"

void adc_init(void)
{
    // disable digital input buffer on PA6 and PA7 (optional, reduces power)
    PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // disable pull-up resistors on PA6 and PA7
    PORTA.PIN6CTRL &= ~PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL &= ~PORT_PULLUPEN_bm;

    // set ADC clock prescaler and reference voltage to VDD (typically 3.3V)
    ADC0.CTRLC = ADC_PRESC_DIV64_gc | ADC_REFSEL_VDDREF_gc;

    // enable ADC and select 10-bit resolution
    ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc;
}

uint16_t adc_read(uint8_t channel)
{
    // select ADC channel
    ADC0.MUXPOS = channel;

    // short delay to allow mux to settle (recommended in datasheet)
    for (volatile uint8_t i = 0; i < 8; i++) {
        __asm__ __volatile__("nop");
    }

    // start conversion
    ADC0.COMMAND = ADC_STCONV_bm;

    // wait for conversion to complete
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));

    // clear result ready flag
    ADC0.INTFLAGS = ADC_RESRDY_bm;

    // return 10-bit result
    return ADC0.RES;
}
