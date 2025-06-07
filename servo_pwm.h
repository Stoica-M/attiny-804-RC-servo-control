/*
 * servo_pwm.h
 *
 * Created: 6/4/2025 11:23:16 PM
 *  Author: stoica
 */ 


#ifndef SERVO_PWM_H_
#define SERVO_PWM_H_

#define SERVO_MIN_US 1000
#define SERVO_MID_US 1500
#define SERVO_MAX_US 2000

#define SERVO_EXT_MAX_US 2400
#define SERVO_EXT_MIN_US 600



void servo_pwm_init(void);
void servo_set_pulse_us(uint8_t channel, uint16_t pulse_us);


#endif /* SERVO_PWM_H_ */