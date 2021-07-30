#include "robot_arm_peripherals.h"


#define STEPPER_DELAY   (5)

void gpio_init(void) {

    //  Enable peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN;

    // Configure GPIOs
    
    // Stepper motor driver pins
    // Configure GPIOB15-GPIOB12 as output push-pull, 50 MHz
    GPIOB->CRH |= GPIO_CRH_MODE15_1 | GPIO_CRH_MODE15_0 | GPIO_CRH_MODE14_1 | GPIO_CRH_MODE14_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0;
    GPIOB->CRH &= ~(GPIO_CRH_CNF15_0 | GPIO_CRH_CNF14_0 | GPIO_CRH_CNF13_0 | GPIO_CRH_CNF12_0);
    
    // ADC pins
    // Configure GPIOA0-GPIOA1 as analog input mode
    GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 | GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    
    // PWM pins
    // Configure GPIOB9-GPIOB8 as alternate function output push-pull, 50 MHz
    GPIOB->CRH |= GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0;
    GPIOB->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0);
   
}



// ADC PERIPHERAL

void adc1_init(void) {
    
    // set ADC clock prescaler to 6
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~(RCC_CFGR_ADCPRE_0);
    
    // enable DMA clock
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    
    // ADCx sample time = 239.5 cycles
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0;
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_0;
    
    // channel sequence length (total number of conversions)
    ADC1->SQR1 |= ADC_SQR1_L_0;
    
    // channel sequence order, default: channel 0 first
    ADC1->SQR3 |= ADC_SQR3_SQ2_0;
    
    // enable scan mode
    ADC1->CR1 |= ADC_CR1_SCAN;
    
    // enable DMA mode
    ADC1->CR2 |= ADC_CR2_DMA;
    
    // give the address to DMA where the data will be fetch
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
    
    // give the address to DMA where the fetched data will be stored
    DMA1_Channel1->CMAR = (uint32_t)adc_samples;
    
    // number of data to transfer
    DMA1_Channel1->CNDTR = 2;
    
    // enable circular mode, memory increment mode
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC | DMA_CCR1_MINC;
    
    // select memory size (size of location array)
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
    
    // select peripheral size (size of data location)
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    
    // enable the DMA
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
    
    // turn on the ADC to wakeup and enable continuos mode
    ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
    delay_ms(1); // delay to account for ADC Tstab
    
    // start ADC calibration
    ADC1->CR2 |= ADC_CR2_CAL;
    while(!(ADC1->CR2 & ADC_CR2_CAL)); // wait for the calibration to complete
    
    // turn on the ADC again to start conversion
    ADC1->CR2 |= ADC_CR2_ADON;
    delay_ms(1); // delay to account for ADC Tstab
}


//  PWM PERIPHERAL

void pwm4_init(uint16_t pwm_freq, uint16_t clk_psc) {
    
    // enable TIMER4 clock
    RCC->APB1ENR |= 1 << 2;
    
    // PWM mode 2, preload enabled
    TIM4->CCMR2 |= (1 << 14) | (1 << 13) | (0 << 12) | (1 << 11);
    TIM4->CCMR2 |= (1 << 6) | (1 << 5) | (0 << 4) | (1 << 3);
    
    // auto-reload preload enabled
    TIM4->CR1 |= 1 << 7;
        
    // prescaler
    TIM4->PSC = clk_psc ;
    
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

void pwm4_duty_cycle(double duty, uint8_t ch) {
    
    switch (ch) {
        case 1:
            TIM4->CCR1 = (uint16_t)((duty * TIM4->ARR) / 100.00);
            delay_ms(2);
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


/*
    GPIOB_12 -> IN1
    GPIOB_13 -> IN2
    GPIOB_14 -> IN3
    GPIOB_15 -> IN4
    
    Sequence
        CW  - 1, 4, 2, 3
        CCW - 4, 1, 3, 2
    
*/

void stepper_direction(uint8_t dir) {
    
    switch (dir) {
        
        case 0:
            GPIOB->BSRR |= GPIO_BSRR_BS15;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR15;
            
            GPIOB->BSRR |= GPIO_BSRR_BS12;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR12;
            
            GPIOB->BSRR |= GPIO_BSRR_BS14;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR14;
            
            GPIOB->BSRR |= GPIO_BSRR_BS13;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR13;
            delay_ms(STEPPER_DELAY);
            break;
        
        case 1:
            GPIOB->BSRR |= GPIO_BSRR_BS12;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR12;
            
            GPIOB->BSRR |= GPIO_BSRR_BS15;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR15;
            
            GPIOB->BSRR |= GPIO_BSRR_BS13;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR13;
            
            GPIOB->BSRR |= GPIO_BSRR_BS14;
            delay_ms(STEPPER_DELAY);
            GPIOB->BSRR |= GPIO_BSRR_BR14;
            delay_ms(STEPPER_DELAY);
            break;
        
        default:
            GPIOB->BSRR |= GPIO_BSRR_BR15 | GPIO_BSRR_BR14 | GPIO_BSRR_BR13 | GPIO_BSRR_BR12;
        }
}
