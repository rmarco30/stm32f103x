/**
 * \file        _template.c
 * \author      Marco, Roldan L.
 * \version     v1.0
 * \date        06/06/2021
 * \brief       stm32f103x project template
 */

#include "stm32f10x.h"
#include "main.h"

void delay(long);
void gpio_ports_enable(void);

void delay(long cycles) {
	while(cycles > 0)
		cycles--;
}

void gpio_ports_enable(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = 0x44344444;
}

int main() {

	gpio_ports_enable();
	
	while(1) {
		
		GPIOC->ODR |= GPIO_ODR_ODR13;
		delay(1000000);
//        printf("1\n");
		GPIOC->ODR &= (uint8_t)(~GPIO_ODR_ODR13);
		delay(1000000);
//        printf("2\n");
	}
}
