#include "prv_periph.h"


void prv_periph_init(void) {
    uart1_init();
    gpio_init();
}


void uart1_init(void) {

/* enable the uart1, alternate function and gpioa peripheral clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

/* configure uart TX pin */
GPIO_InitTypeDef gpio_uart_pins;
memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins)); // initialize to zero each member of structure

gpio_uart_pins.GPIO_Pin = GPIO_Pin_9;               // PA9 (uart1 TX)
gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF_OD;         // configure as alternate function
gpio_uart_pins.GPIO_Speed = GPIO_Speed_50MHz;       // gpio speed 50MHz
GPIO_Init(GPIOA, &gpio_uart_pins);                  // initialize gpioa

/* configure uart1 */
USART_InitTypeDef uart1_init;
memset(&uart1_init, 0, sizeof(uart1_init));         // initialize to zero each member of structure
uart1_init.USART_BaudRate = 9600;                   // baudrate 9600
uart1_init.USART_Mode = USART_Mode_Tx;              // uart tx enabled
USART_Init(USART1, &uart1_init);                    // initialize uart1
USART_Cmd(USART1, ENABLE);                          // enable usart1

}

void uart1_tx(char *msg) {
    
    for(uint32_t i=0; i < strlen(msg); i++) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == 0);
        USART_SendData(USART1, msg[i]);
    }
     
}

void gpio_init(void) {
    
    /* enable GPIOB and GPIOC peripheral clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 
    
    /* configure led pin */
    GPIO_InitTypeDef led;
    led.GPIO_Pin = GPIO_Pin_13;
    led.GPIO_Mode = GPIO_Mode_Out_PP;
    led.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &led);
    
    /* configure button pin */
    GPIO_InitTypeDef button;
    button.GPIO_Pin = GPIO_Pin_0;
    button.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &button);
    
    
}
