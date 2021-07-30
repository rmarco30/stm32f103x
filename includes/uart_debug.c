#include "uart_debug.h"
#include "drv_gpio.h"


void uart_debug_init(void) {
    
    // enable GPIOA, Alternate Function, USART1 clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN;
    
    // configure UART1 TX pin
    gpio_pin_conf_t uart_pins;
    uart_pins.mode = GPIO_MODE_OUTPUT_50MHZ;
    uart_pins.cnf = GPIO_CNF_OUTPUT_ALTF_PP;
    gpio_init(GPIOA, 9, &uart_pins);
    
    
    // configure UART peripheral
    
    /**
     *  baud_rate = clk_freq / (16 * USARTDIV)
     *  USARTDIV = DIV_Mantissa[11:0] and DIV_Fraction[3:0]
     *  9600 = 72MHz / (16 * USARTDIV)
     *  USARTDIV = 468.75
     *  DIV_Mantissa[11:0] = 468 = 0x1d4
     *  DIV_Fraction[3:0] = 0.75 * 16 = 12 = 0xC
     *  USART_BRR = 0x1d4c
     *  
     */
    USART1->BRR = 0x1d4c;
    
    // enable uart transmitter
    USART1->CR1 |= USART_CR1_TE;
    
    // enable uart
    USART1->CR1 |= USART_CR1_UE;

}


void uart_tx_str(char *str) {
    for (uint8_t i=0; str[i] != '\0'; i++) {
        USART1->DR = str[i];
        while(!(USART1->SR & USART_SR_TXE));
    }
}


#if 0

// for future reference

void uart_tx_char(char ch) {
        USART1->DR = ch;
        while(!(USART1->SR & USART_SR_TXE));
}

void uart_tx_str(char *str) {
    for (uint8_t i=0; str[i] != '\0'; i++) {
        uart_tx_char(str[i]);
    }
}

#endif
