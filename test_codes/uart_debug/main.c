/**
 * \file        main.c
 * \author      Marco, Roldan L.
 * \version     v1.0
 * \date        06/06/2021
 * \brief       uart_debug test code
 */

#include "stm32f10x.h"
#include "uart_debug.h"
#include "drv_systick_delay.h"

int main() {
    
    uart_debug_init();
    systick_init();
 
    while(1) {
    
        uart_tx_str("hello world!\n");
        delay_ms(1000);
    }
}
