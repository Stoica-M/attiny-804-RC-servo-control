/*
 * adc.h
 *
 * Created: 06/04/2024 22:29:06
 *  Author: stoica
 */ 


#ifndef ADC_H_
#define ADC_H_


#include <stdbool.h>
#include <avr/io.h>

void adc_init(void);
uint16_t adc_read(uint8_t channel);


#endif /* ADC_H_ */