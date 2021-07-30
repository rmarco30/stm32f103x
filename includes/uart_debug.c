#include "uart_debug.h"


void uart_debug_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN;
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;
    GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);
    
    USART1->BRR = 0x1d4c;
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_UE;
}

//void uart_tx_char(char ch) {
//        USART1->DR = ch;
//        while(!(USART1->SR & USART_SR_TXE));
//}

//void uart_tx_str(char *str) {
//    for (uint8_t i=0; str[i] != '\0'; i++) {
//        uart_tx_char(str[i]);
//    }
//}

void uart_tx_str(char *str) {
    for (uint8_t i=0; str[i] != '\0'; i++) {
        USART1->DR = str[i];
        while(!(USART1->SR & USART_SR_TXE));
    }
}
