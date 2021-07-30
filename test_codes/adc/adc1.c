#include "stm32f10x.h"
#include "drv_systick_delay.h"
#include "adc1.h"


void adc1_init(void) {
    
    /* GPIOA configuration */
    
    // enable GPIOA and Alternate function clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    
    // configure PORTA0 and PORTA1 as input in analog mode
    GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 | GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    
    
    
    /* DMA configuration */
    
    // enable DMA clock
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    
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
    
    
    
    /* ADC configuration */
    
    // enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    // set ADC clock prescaler to 6
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~(RCC_CFGR_ADCPRE_0);
    
    // ADCx sample time = 239.5 cycles
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0;
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_0;
    
    // channel sequence length (total number of conversions)
    ADC1->SQR1 |= ADC_SQR1_L_0;
    
    // channel sequence order, default: channel 0 first
    //ADC1->SQR3 &= ~(ADC_SQR3_SQ1_4 | ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_0);
    ADC1->SQR3 |= ADC_SQR3_SQ2_0;
    
    #if 0 // enable this if interrupt will be used
    // enable End of Conversion interrupt
    ADC1->CR1 |= ADC_CR1_EOCIE;
    NVIC_EnableIRQ(ADC1_2_IRQn);
    #endif
    
    // enable scan mode
    ADC1->CR1 |= ADC_CR1_SCAN;
    
    // enable DMA mode
    ADC1->CR2 |= ADC_CR2_DMA;
    
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


#if 0 // enable this if interrupt will be used
void ADC1_2_IRQHandler(void) {
    if (ADC1->SR & ADC_SR_EOC) {
       adc_val = ADC1->DR;
    }
}
#endif
