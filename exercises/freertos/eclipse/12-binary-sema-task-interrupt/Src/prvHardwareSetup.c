#include "stm32f10x.h"
#include "prvHardwareSetup.h"
#include "string.h"

void prvHardwareSetup( void ) {
    prvConfigureUART1();
    prvConfigureGPIO();
}


void prvConfigureUART1( void ) {

/* enable the uart1, alternate function and gpioa peripheral clock */
RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE );

/* configure uart TX pin */
GPIO_InitTypeDef GPIO_Init_uart1_tx;
memset( &GPIO_Init_uart1_tx, 0, sizeof( GPIO_Init_uart1_tx ) ); /* initialize to zero each member of structure */

GPIO_Init_uart1_tx.GPIO_Pin = GPIO_Pin_9;
GPIO_Init_uart1_tx.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_Init_uart1_tx.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init( GPIOA, &GPIO_Init_uart1_tx );

GPIO_InitTypeDef GPIO_Init_uart1_rx;
memset( &GPIO_Init_uart1_rx, 0, sizeof(GPIO_Init_uart1_rx) );
GPIO_Init_uart1_rx.GPIO_Pin = GPIO_Pin_10;
GPIO_Init_uart1_rx.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init( GPIOA, &GPIO_Init_uart1_rx );

/* configure uart1 */
USART_InitTypeDef USART1_Init_uart1;
memset( &USART1_Init_uart1, 0, sizeof( USART1_Init_uart1 ) ); /* initialize to zero each member of structure */
USART1_Init_uart1.USART_BaudRate = 9600;
USART1_Init_uart1.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
USART_Init( USART1, &USART1_Init_uart1 );

/* uart1 interrupt configuration */
USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );

NVIC_SetPriority( USART1_IRQn, 5 );
NVIC_EnableIRQ( USART1_IRQn );
    
USART_Cmd( USART1, ENABLE );

}


void prvConfigureGPIO( void )
{
    
    /* enable GPIOB and GPIOC peripheral clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE ); 
    
    /* configure led pin */
    GPIO_InitTypeDef GPIO_Init_led;
    GPIO_Init_led.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init_led.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init_led.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_Init_led );
    
    /* configure button pin */
    GPIO_InitTypeDef GPIO_Init_button;
    GPIO_Init_button.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init_button.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init( GPIOB, &GPIO_Init_button );
    
    #if 1
    /* GPIO_Init_button interrupt configuration */
    
    /* configure EXTI line to be used in AFIO register */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    
    /* configure EXTI registers for required behavior */
    EXTI_InitTypeDef sEXTI_Init_pb0;
    sEXTI_Init_pb0.EXTI_Line = EXTI_Line0;
    sEXTI_Init_pb0.EXTI_Mode = EXTI_Mode_Interrupt;
    sEXTI_Init_pb0.EXTI_Trigger = EXTI_Trigger_Falling;
    sEXTI_Init_pb0.EXTI_LineCmd = ENABLE;
    EXTI_Init(&sEXTI_Init_pb0);
    
    /* configure NVIC register for enabling and setting priority of interrupt */
    NVIC_SetPriority(EXTI0_IRQn, 5);
    NVIC_EnableIRQ(EXTI0_IRQn);
    #endif
}
