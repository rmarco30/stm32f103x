/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 01, 2021
  * @brief   freertos using keil test code
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "prv_periph.h"
#include <stdio.h>


/* Function Prototypes */
void vLed_TaskHandler1( void *pvParam );
void vLed_TaskHandler2( void *pvParam );
void vRTOS_delay( uint32_t ulTicks_in_ms );


/* Globals */
uint8_t ucLed_status_flag = 0;

/* Task Handles */
TaskHandle_t xLed_TaskHandle1 = NULL;
TaskHandle_t xLed_TaskHandle2 = NULL;

int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvPeriph_Init();
    
    /* tasks */
	xTaskCreate( vLed_TaskHandler1, "led-task", 1000, NULL, 1, &xLed_TaskHandle1 );
    xTaskCreate( vLed_TaskHandler2, "button-task", 1000, NULL, 2, &xLed_TaskHandle2 );
    
    /* start the scheduler */
	vTaskStartScheduler();
    
	for( ;; );
}

void vLed_TaskHandler1( void *pvParam )
{
	while( 1 )
    {
        ucLed_status_flag ^= 1;
        vRTOS_delay(200);
        
        /* toggle the led */
        switch( ucLed_status_flag )
        {
        case 0:
            GPIO_WriteBit( GPIOC, GPIO_Pin_13, RESET );
            break;
        
        case 1:
            GPIO_WriteBit( GPIOC, GPIO_Pin_13, SET );
            break;
        }
    }
}

void vLed_TaskHandler2( void *pvParam )
{
    while( 1 )
    {
        if( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_0 ) == 0 )
        {
            vRTOS_delay(150);
            vTaskDelete( xLed_TaskHandle2 );
        }
        else
        {
            ucLed_status_flag ^= 1;
            vRTOS_delay(1000);
            
            /* toggle the led */
            switch( ucLed_status_flag )
            {
            case 0:
                GPIO_WriteBit( GPIOC, GPIO_Pin_13, RESET );
                break;
            
            case 1:
                GPIO_WriteBit( GPIOC, GPIO_Pin_13, SET );
                break;
            }
        }
    }
}



/**
 @name   vRTOS_delay
 @brief  blocking delay that waits in milliseconds
 @param  ulTicks_in_ms - time to wait in milliseconds
 @retval none
*/
void vRTOS_delay( uint32_t ulTicks_in_ms )
{
    uint32_t ulCurrent_tick_count = xTaskGetTickCount();
    uint32_t ulDelay_in_ticks = ( ulTicks_in_ms * configTICK_RATE_HZ ) / 1000;
    while( xTaskGetTickCount() < ( ulCurrent_tick_count + ulDelay_in_ticks ) );
}
