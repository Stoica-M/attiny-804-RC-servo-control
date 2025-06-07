/*
 * pwm_input.h
 *
 * Created: 6/3/2025 12:41:13 PM
 *  Author: stoica
 */ 

// pwm_input.h
#ifndef PWM_INPUT_H_
#define PWM_INPUT_H_

#include <stdint.h>
#include <stdbool.h>

void pwm_input_init(void);
uint16_t pwm_input_get_ch1_us(void);
uint16_t pwm_input_get_ch2_us(void);
bool pwm_input_ch1_in_range(uint16_t min_us, uint16_t max_us);
bool pwm_input_ch2_in_range(uint16_t min_us, uint16_t max_us);

#endif /* PWM_INPUT_H_ */
