/**
 * \file        main.c
 * \author      Marco, Roldan L.
 * \version     v1.0
 * \date        05/06/2021
 * \brief       pwm test code
 */

#include "stm32f10x.h"
#include "pwm.h"
#include "drv_systick_delay.h"

int main() {
    
    systick_init();
    pwm4_init(50);
    
	while(1) {
        
        for (int i = 0; i < 100; i++) {
            pwm4_duty_cycle(i, 4);
            delay_ms(100);
        }
	}
}
