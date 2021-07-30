#ifndef __ADC1_H
#define __ADC1_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "stdint.h"


extern volatile uint32_t adc_val;
extern uint16_t adc_samples[2];

void adc1_init(void);

#if 0 // enable this if interrupt will be used
void ADC1_2_IRQHandler(void);
#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ADC1_H
