/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 6, 2021
  * @brief   Led task will be deleted on button press and will be indicated by
  *          faster toggling of led. Note: due to blocking nature of the delay
  *          used, the button must be hold for a while.
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"
#include <stdio.h>

/* Defines */


/* Function Prototypes */
void vLed_TaskHandler( void *pvParam );
void vButton_TaskHandler( void *pvParam );
void vRTOS_delay( uint32_t ulTicks_in_ms );


/* Globals */
uint8_t ucLed_status_flag = 0;

/* Task Handles */
TaskHandle_t xLed_TaskHandle = NULL;
TaskHandle_t xButton_TaskHandle = NULL;

int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* tasks */
    xTaskCreate( vLed_TaskHandler, "led-task", 1000, NULL, 1, &xLed_TaskHandle );
    xTaskCreate( vButton_TaskHandler, "button-task", 1000, NULL, 2, &xButton_TaskHandle );

    /* start the scheduler */
    vTaskStartScheduler();

    for( ;; );
}

void vLed_TaskHandler( void *pvParam )
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

void vButton_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        if( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_0 ) == 0 )
        {
            vRTOS_delay(150);
            vTaskDelete( xButton_TaskHandle );
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
 @brief  delay that waits in milliseconds
 @param  ulTicks_in_ms - time to wait in milliseconds
 @retval none
*/
void vRTOS_delay( uint32_t ulTicks_in_ms )
{
    uint32_t ulCurrent_tick_count = xTaskGetTickCount();
    uint32_t ulDelay_in_ticks = ( ulTicks_in_ms * configTICK_RATE_HZ ) / 1000;
    while( xTaskGetTickCount() < ( ulCurrent_tick_count + ulDelay_in_ticks ) );
}
