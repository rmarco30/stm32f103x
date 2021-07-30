#include "drv_systick_delay.h"


static volatile uint32_t time_delay = 0;

void SysTick_Handler(void) {
    if(time_delay > 0 )
        time_delay--;               // every SysTick interrupt decrement global variable time_delay
}

void delay_us(uint32_t us) {		// nTime: specifies the delay time length		
	time_delay = us;				// time_delay must be declared volatile
	while(time_delay != 0);			// busy wait
}

void delay_ms(uint32_t ms) {			
	while(ms--) {
        delay_us(1000);
    }
}

void systick_init(void) {
    uint32_t ticks = SystemCoreClock / 1000000;
	SysTick->CTRL = 0;				// disable SysTick
	SysTick->LOAD = ticks - 1;		// set reload register

	// set interrupt priority of SysTick to least urgency (i.e., largest priority value)
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;				// reset the SysTick counter value

	// Select processor clock: 1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

	// enables SysTick interrupt, 1 = enable, 0 = disable
	SysTick->CTRL |= SysTick_CTRL_TICKINT;

	// enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE;

//     SysTick_Config(SystemCoreClock / 1000000);
}
