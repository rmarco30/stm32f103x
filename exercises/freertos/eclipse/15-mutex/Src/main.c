/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 9, 2021
  * @brief   Avoiding race condition using mutex.
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"
#include <stdio.h>

/* Defines */


/* Function Prototypes */
void vRTOS_delay( uint32_t ulTicks_in_ms );

/* Task Handlers */
void vTask1( void *pvParam );
void vTask2( void *pvParam );

/* Task Handles */

/* Semaphores Handle*/
SemaphoreHandle_t xMutex_Handle;

/* Globals */
uint32_t shared_var;

int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* create tasks */
    xTaskCreate( vTask1, "task1", 500, NULL, 2, NULL );
    xTaskCreate( vTask2, "task2", 500, NULL, 3, NULL );

    /* create semaphore */
    xMutex_Handle = xSemaphoreCreateMutex();

    if( xMutex_Handle == NULL )
    {
        prvUART1_Write("Semaphore Creation Error!");
        /* Hang here */
        while( 1 );
    }

    /* start the scheduler */
    vTaskStartScheduler();

    /* program execution should not reach here */
    for( ;; );
}


void vTask1( void *pvParam )
{
    while( 1 )
    {
        if( xSemaphoreTake( xMutex_Handle, portMAX_DELAY ) == pdTRUE )
        {
            char msg[30];
            for( uint8_t i = 0; i < 5; i++ )
            {
                shared_var++;
                sprintf( msg, "Task1, shared_var value: %lu\n", shared_var );
                prvUART1_Write( msg );
            }
            xSemaphoreGive( xMutex_Handle );
        }
        else
        {
            /* do something else if mutex is not available */
        }
    }
}


void vTask2( void *pvParam )
{
    while( 1 )
    {
        if( xSemaphoreTake( xMutex_Handle, 10 ) == pdTRUE )
        {
            char msg[30];
            for( uint8_t i = 0; i < 10; i++ )
            {
                shared_var++;
                sprintf( msg, "Task2, shared_var value: %lu\n", shared_var );
                prvUART1_Write( msg );
            }
            xSemaphoreGive( xMutex_Handle );
            vTaskDelay( pdMS_TO_TICKS(100) );
        }
        else
        {
            /* do something else if mutex is not available */
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
