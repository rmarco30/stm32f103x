#include "stm32f10x.h"
#include "drv_systick-delay.h"

void delay(long);
void gpio_ports_enable(void);

void delay(long cycles) {
	while(cycles > 0)
		cycles--;
}

void gpio_ports_enable(void){
    
    // enable GPIOC clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    // configure gpio_13 to output mode 50MHz
    GPIOC->CRH |= (1U << 20) | (1U << 21);
    
    // configure gpio_13 to general purpose output push-pull
    GPIOC->CRH &= ~((1U << 22) | (1U << 23));
    
    // initialize systick_delay
    systick_init();
}

int main() {
    
	gpio_ports_enable();
	
	while(1) {
		
		GPIOC->ODR |= GPIO_ODR_ODR13;
		delay_ms(1000);
		GPIOC->ODR &= (uint8_t)(~GPIO_ODR_ODR13);
		delay_ms(1000);
	}
}
