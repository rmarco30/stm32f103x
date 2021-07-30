#ifndef __ROBOT_ARM_PERIPH_H
#define __ROBOT_ARM_PERIPH_H

#include "stm32f10x.h"
#include "stdint.h"
#include "systick-delay.h"


//  GPIO
void gpio_init(void);

//  ADC
void adc1_init(void);
extern uint16_t adc_samples[2];
extern volatile uint32_t adc_val;

//  PWM
void pwm4_init(uint16_t pwm_freq, uint16_t clk_psc);
void pwm4_duty_cycle(double duty, uint8_t ch);

//  STEPPER
void stepper_direction(uint8_t dir);





#endif // __ROBOT_ARM_PERIPH_H
