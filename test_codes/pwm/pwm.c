#include "stm32f10x.h"
#include "pwm.h"

void pwm4_init(uint16_t pwm_freq) {
    
    // enable GPIOB clock and alternate function
    RCC->APB2ENR |= (1 << 3) | (1 << 0);
    // configure GPIOB9 as 50 MHz alternate function output push-pull
    GPIOB->CRH |= (1 << 7) | (0 << 6) | (1 << 5) | (1 << 4);
    GPIOB->CRH |= (1 << 3) | (0 << 2) | (1 << 1) | (1 << 0);
    
    // enable TIMER4 clock
    RCC->APB1ENR |= 1 << 2;
    
    // PWM mode 2, preload enabled
    TIM4->CCMR2 |= (1 << 14) | (1 << 13) | (0 << 12) | (1 << 11);
    TIM4->CCMR2 |= (1 << 6) | (1 << 5) | (0 << 4) | (1 << 3);
    // auto-reload preload enabled
    TIM4->CR1 |= 1 << 7;
        
    // prescaler
    TIM4->PSC = (SystemCoreClock / 1000000);
    
    if (pwm_freq == 0) {
        pwm_freq = 1;
    }
    // frequency
    TIM4->ARR = (uint16_t)(SystemCoreClock / (pwm_freq * TIM4->PSC));
    
    // duty cycle
    TIM4->CCR4 = TIM4->ARR >> 1;
    
    // enable OC4 output
    TIM4->CCER |= 1 << 12;
    TIM4->CCER |= 1 << 8;
    
    // set UG bit to initialize registers
    TIM4->EGR |= 1 << 0;
    // counter enabled
    TIM4->CR1 |= 1 << 0;
}


void pwm4_duty_cycle(double duty, uint8_t channel) {
    
    switch (channel) {
        case 1:
            TIM4->CCR1 = (uint16_t)((duty * TIM4->ARR) / 100.00);
            break;
        case 2:
            TIM4->CCR2 = (uint16_t)((duty * TIM4->ARR) / 100.00);
            break;
        case 3:
            TIM4->CCR3 = (uint16_t)((duty * TIM4->ARR) / 100.00);
            break;
        case 4:
            TIM4->CCR4 = (uint16_t)((duty * TIM4->ARR) / 100.00);
            break;
    }
}
