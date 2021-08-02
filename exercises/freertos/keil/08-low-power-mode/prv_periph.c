#include "prv_periph.h"
#include "string.h"


void prvPeriph_Init( void ) {
    prvUART1_Init();
    prvGPIO_Init();
}


static void prvUART1_Init( void ) {

/* enable the uart1, alternate function and gpioa peripheral clock */
RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE );

/* configure uart TX pin */
GPIO_InitTypeDef GPIO_Init_uart1;
memset( &GPIO_Init_uart1, 0, sizeof( GPIO_Init_uart1 ) ); /* initialize to zero each member of structure */

GPIO_Init_uart1.GPIO_Pin = GPIO_Pin_9;
GPIO_Init_uart1.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_Init_uart1.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init( GPIOA, &GPIO_Init_uart1 );

/* configure uart1 */
USART_InitTypeDef USART1_Init_uart1;
memset( &USART1_Init_uart1, 0, sizeof( USART1_Init_uart1 ) ); /* initialize to zero each member of structure */
USART1_Init_uart1.USART_BaudRate = 9600;
USART1_Init_uart1.USART_Mode = USART_Mode_Tx;
USART_Init( USART1, &USART1_Init_uart1 );
USART_Cmd( USART1, ENABLE );

}

void prvUART1_Write( char *cStr )
{
    for( uint32_t i = 0; i < strlen( cStr ); i++ )
    {
        while( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == 0 );
        USART_SendData( USART1, cStr[i] );
    }
}

static void prvGPIO_Init( void )
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
    
    #if 0
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
    NVIC_SetPriority(EXTI0_IRQn, 4);
    NVIC_EnableIRQ(EXTI0_IRQn);
    #endif
}
