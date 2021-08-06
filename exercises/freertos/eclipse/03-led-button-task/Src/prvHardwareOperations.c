#include "stm32f10x.h"
#include "prvHardwareOperations.h"
#include "string.h"

/**
 @name   prvGPIO_ToggleBit
 @brief  Toggles the selected GPIO output pin
 @param  GPIOx: Any of the GPIO_TypeDef* available (GPIOA, GPIOB, GPIOC, ..)
 @param  GPIO_Pin: Any of the GPIO_Pin_x where x is the pin number
 @retval none
*/
void prvUART1_Write( char *cStr )
{
    for( uint32_t i = 0; i < strlen( cStr ); i++ )
    {
        while( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == 0 );
        USART_SendData( USART1, cStr[i] );
    }
}



/**
 @name   prvGPIO_ToggleBit
 @brief  Toggles the selected GPIO output pin
 @param  GPIOx: Any of the GPIO_TypeDef* available (GPIOA, GPIOB, GPIOC, ..)
 @param  GPIO_Pin: Any of the GPIO_Pin_x where x is the pin number
 @retval none
*/
void prvGPIO_ToggleBit( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin )
{
    uint8_t ucPin_value = 0;
    ucPin_value = GPIO_ReadOutputDataBit( GPIOx, GPIO_Pin );
    ucPin_value ^= 1;
    
    /* toggle the led */
    switch( ucPin_value )
    {
    case 0:
        GPIO_WriteBit( GPIOx, GPIO_Pin, Bit_RESET );
        break;

    case 1:
        GPIO_WriteBit( GPIOx, GPIO_Pin, Bit_SET );
        break;
    }
}
