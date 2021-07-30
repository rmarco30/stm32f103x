#include "stm32f10x.h"
#include "systick-delay.h"
#include "robot_arm_peripherals.h"
#include "functions.h"
#include "uart_debug.h"

uint16_t adc_samples[2];
static char res[50];


int main() {
    systick_init();
    gpio_init();
    adc1_init();
    pwm4_init(50, 72);
    uart_debug_init();
	
 	while(1) {
        
        if (adc_samples[0] < 500) {
            stepper_direction(0);
        }
        else if (adc_samples[0] > 3500) {
            stepper_direction(1);
        }
        else {
            stepper_direction(2);
        }
        
        double duty_val = 0;
        duty_val = fmap((uint16_t)adc_samples[0], 0, 4095, 0, 16);
        pwm4_duty_cycle((double)duty_val, 4);
        
  
        sprintf(res, "channel 0: %hu,   ", adc_samples[0]);
        uart_tx_str(res);
        sprintf(res, "duty cycle: %lf\n", duty_val);
        uart_tx_str(res);
	}
}
