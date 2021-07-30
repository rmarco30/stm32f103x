#include "gpio.h"


void gpio_init(void) {

	GPIOB_CRL = (1 << 0) | (1 << 1) | (1 << 4) | (1 << 5);             // configure port B0 and B1 as output
    GPIOB_CRH = (1 << 8) | (1 << 9);                                   // configure port B10 as output
}

void turn_on_led(uint32_t pin) {

	GPIOB_ODR |= pin;
}

void turn_off_led(uint32_t pin) {
	
	GPIOB_ODR &= ~pin;
}