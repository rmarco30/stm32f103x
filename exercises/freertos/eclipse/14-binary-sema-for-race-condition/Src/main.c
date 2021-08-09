/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 9, 2021
  * @brief   Synchronization of task and interrupt using binary semaphore.
  *          Using binary semaphore as to guard a shared resource.
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
SemaphoreHandle_t xSema_Handle;

/* Globals */


int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* create tasks */
    xTaskCreate( vTask1, "task1", 500, NULL, 2, NULL );
    xTaskCreate( vTask2, "task2", 500, NULL, 2, NULL );

    /* create semaphore */
    xSema_Handle = xSemaphoreCreateBinary();

    if( xSema_Handle == NULL )
    {
        prvUART1_Write("Semaphore Creation Error!");
        /* Hang here */
        while( 1 );
    }

    xSemaphoreGive( xSema_Handle );

    /* start the scheduler */
    vTaskStartScheduler();

    /* program execution should not reach here */
    for( ;; );
}


void vTask1( void *pvParam )
{
    while( 1 )
    {
        xSemaphoreTake( xSema_Handle, portMAX_DELAY );
        prvUART1_Write( "Task 1 running!\n" );
        xSemaphoreGive( xSema_Handle );
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void vTask2( void *pvParam )
{
    while( 1 )
    {
        xSemaphoreTake( xSema_Handle, portMAX_DELAY );
        prvUART1_Write( "Task 2 running!\n" );
        xSemaphoreGive( xSema_Handle );
        vTaskDelay(pdMS_TO_TICKS(100));
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
