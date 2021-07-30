#ifndef __UART_DEBUG_H
#define __UART_DEBUG_H

#include "stm32f10x.h"
#include "stdio.h"

void uart_debug_init(void);
// void uart_tx_char(char data);
void uart_tx_str(char *str);


/*

    format specifiers
    
    %c   - character
    %d   - signed integer
    %e   - scientific notation of floats
    %f   - float
    %hi  - signed integer (short)
    %hu  - unsigned integer (short)
    %l   - long
    %lf  - double
    %Lf  - long double
    %lu  - unsigned int or unsigned long
    %lli - Long long
    %o   - octal
    %p   - pointer
    %s   - string
    %u   - unsigned int
    %x   - hexadecimal
    %n   - prints nothing
    %%   - prints % character
    
    sample usage:
    
        #include "stdio.h"
        
        static char res[50];
        sprintf(res, "%lf\n", duty_val);
        uart_tx_str(res);

*/


#endif // __UART_DEBUG_H
