#ifndef __ADC1_H
#define __ADC1_H

#include "stdint.h"


extern volatile uint32_t adc_val;
extern uint16_t adc_samples[2];

void adc1_init(void);
void ADC1_2_IRQHandler(void);

#endif // __ADC1_H
