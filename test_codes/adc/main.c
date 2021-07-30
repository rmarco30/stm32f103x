/**
 * \file        main.c
 * \author      Marco, Roldan L.
 * \version     v1.0
 * \date        06/06/2021
 * \brief       ADC with DMA test code 
 */

#include "stm32f10x.h"
#include "drv_systick_delay.h"
#include "pwm.h"
#include "adc1.h"
#include "uart_debug.h"
#include "prv_functions.h"
#include "drv_gpio.h"


volatile uint32_t adc_val;
uint16_t adc_samples[2] = {0, 0};

static char res[50];

int main() {
       
    systick_init();
    pwm4_init(50);
    adc1_init();
    uart_debug_init();

    while(1) {
        
        // map the 12-bit adc value to 0-100 to be fed to pwm to control the brightness of led
        double duty_val = 0;
        duty_val = fmap((uint16_t)adc_samples[0], 0, 4095, 0, 100);
        pwm4_duty_cycle((double)duty_val, 4);
        
        // print the duty cycle for debugging
        sprintf(res, "Channel 3: %lf\n", duty_val);
        uart_tx_str(res);
        
        
//        sprintf(res, "Channel 0: %hu     ", adc_samples[0]);
//        uart_tx_str(res);;
//        sprintf(res, "Channel 1: %hu\n", adc_samples[1]);
//        uart_tx_str(res);
    }
}

