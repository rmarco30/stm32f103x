#include "stm32f10x.h"
#include <stdio.h>

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
    
    SystemCoreClockUpdate();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	gpio_ports_enable();
	
	while(1) {
		
		GPIOC->ODR |= GPIO_ODR_ODR13;
//		delay(1000000);
//        printf("1\n");
		GPIOC->ODR &= (uint32_t)(~GPIO_ODR_ODR13);
//		delay(1000000);
//        printf("2\n");
	}
}

