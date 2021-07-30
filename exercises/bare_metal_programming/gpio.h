#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIO_BASE      	0x40010C00U
#define GPIOB_CRL       (*((volatile unsigned int *)(GPIO_BASE)))
#define GPIOB_CRH       (*((volatile unsigned int *)(GPIO_BASE + 0x04U)))
#define GPIOB_ODR       (*((volatile unsigned int *)(GPIO_BASE + 0x0CU)))


void gpio_init(void);
void turn_on_led(uint32_t pin);
void turn_off_led(uint32_t pin);



#endif