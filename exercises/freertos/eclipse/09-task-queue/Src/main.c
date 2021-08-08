/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 6, 2021
  * @brief   One task will send data into queue and another is waiting for data
  *          in the queue.
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"
#include <stdio.h>

/* Defines */


/* Function Prototypes */
void vBlocking_delay( uint32_t ulTicks_in_ms );

/* Task Handlers */
void vTask1_TaskHandler( void *pvParam );
void vTask2_TaskHandler( void *pvParam );
void vApplicationIdleHook( void );

/* Task Handles */
TaskHandle_t xTask1_TaskHandle = NULL;
TaskHandle_t xTask2_TaskHandle = NULL;

/* Queue Handles */
static QueueHandle_t xQueue1;

/* Globals */
static uint32_t ulVar = 0;

int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* queues */
    xQueue1 = xQueueCreate( 1, sizeof(uint32_t) );

    if( xQueue1 == NULL )   /* checks if queue creation was succesful */
    {
        return 0;
    }

    /* tasks */
    xTaskCreate( vTask1_TaskHandler, "task-1", 500, NULL, 3, &xTask1_TaskHandle );
    xTaskCreate( vTask2_TaskHandler, "task-2", 500, NULL, 2, &xTask2_TaskHandle );


    /* start the scheduler */
    vTaskStartScheduler();

    /* program should not reach here */
    for( ;; );
}


void vTask1_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        if( xQueueSend( xQueue1, &ulVar, pdMS_TO_TICKS(100)) != pdTRUE )
        {
            prvUART1_Write("QUEUE FULL\n");
        }
        else
        {
            ulVar++;
        }
    }
}


void vTask2_TaskHandler( void *pvParam )
{
    uint32_t xBuffer;
    char msg[20];
    while( 1 )
    {
        if( xQueueReceive( xQueue1, &xBuffer, 0 ) == pdTRUE )
        {
            sprintf( msg, "%lu", xBuffer );
            prvUART1_Write( "received: " );
            prvUART1_Write( msg );
            prvUART1_Write( "\n" );
        }
        else
        {
            prvUART1_Write( "Queue empty\n");
        }
    }
}


/**
 @name   vBlocking_delay
 @brief  blocking delay that waits in milliseconds
 @param  ulTicks_in_ms - time to wait in milliseconds
 @retval none
*/
void vBlocking_delay( uint32_t ulTicks_in_ms )
{
    uint32_t ulCurrent_tick_count = xTaskGetTickCount();
    uint32_t ulDelay_in_ticks = ( ulTicks_in_ms * configTICK_RATE_HZ ) / 1000;
    while( xTaskGetTickCount() < ( ulCurrent_tick_count + ulDelay_in_ticks ) );
}
