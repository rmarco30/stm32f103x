#ifndef __PWM_H
#define __PWM_H


#include "stdint.h"

/*
 Timer4, channel 4 PWM mode, edge-aligned, up-counting
 Prescaler = 72
*/

void pwm4_init(uint16_t pwm_freq, uint16_t clk_psc);
void pwm4_duty_cycle(double duty, uint8_t ch);


#endif // __PWM_H
