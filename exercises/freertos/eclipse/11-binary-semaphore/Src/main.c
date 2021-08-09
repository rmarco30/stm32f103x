/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 8, 2021
  * @brief   Synchronization of tasks using binary semaphore
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"
#include <stdio.h>
#include <stdlib.h>

/* Defines */


/* Function Prototypes */
void vRTOS_delay( uint32_t ulTicks_in_ms );

/* Task Handlers */
void vProducer_TaskHandler( void *pvParam );
void vConsumer1_TaskHandler( void *pvParam );
void vConsumer2_TaskHandler( void *pvParam );

/* Task Handles */
TaskHandle_t xProducer_TaskHandle = NULL;
TaskHandle_t xConsumer1_TaskHandle = NULL;
TaskHandle_t xConsumer2_TaskHandle = NULL;

/* Queue Handles */
QueueHandle_t xData_QueueHandle = NULL;

/* Semaphores Handle*/
SemaphoreHandle_t xSema_Handle;

/* Globals */


int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* create tasks */
    xTaskCreate( vProducer_TaskHandler, "producer-task", 500, NULL, 2, &xProducer_TaskHandle );
    xTaskCreate( vConsumer1_TaskHandler, "consumer1-task", 500, NULL, 3, &xConsumer1_TaskHandle );
    xTaskCreate( vConsumer2_TaskHandler, "consumer2-task", 500, NULL, 3, &xConsumer2_TaskHandle );

    /* create queue */
    xData_QueueHandle = xQueueCreate( 4, sizeof( uint32_t ) );

    if( xData_QueueHandle == NULL )
    {
        prvUART1_Write("Queue Creation Error!");
        /* Hang here */
        while( 1 );
    }

    /* create semaphore */
    xSema_Handle = xSemaphoreCreateBinary();

    if( xSema_Handle == NULL )
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


void vProducer_TaskHandler( void *pvParam )
{
    uint32_t xData;
    BaseType_t xSend_status;

    while( 1 )
    {
        taskENTER_CRITICAL();
        xData = ( uint32_t )( rand() & 0xff );
        xSend_status = xQueueSend( xData_QueueHandle, ( void * )&xData, portMAX_DELAY );
        char data[16];
        sprintf( data, "Data Ready: %lu\n", xData );
        prvUART1_Write( data );
        taskEXIT_CRITICAL();

        if( xSend_status == pdTRUE )
        {
            xSemaphoreGive( xSema_Handle );
        }
        else
        {
            prvUART1_Write("Data queueing failed\n");
        }
    }
}


void vConsumer1_TaskHandler( void *pvParam )
{
    uint32_t xData_received;
    BaseType_t xReceive_status;

    while( 1 )
    {
        while( xSemaphoreTake( xSema_Handle, portMAX_DELAY ) == pdTRUE )
        {
            xReceive_status = xQueueReceive( xData_QueueHandle, &xData_received, portMAX_DELAY );
            if( xReceive_status == pdTRUE )
            {
                char buff[15];
                sprintf( buff, "Consumer1: %lu\n", xData_received );
                prvUART1_Write( buff );
            }
        }
    }
}


void vConsumer2_TaskHandler( void *pvParam )
{
    uint32_t xData_received;
    BaseType_t xReceive_status;

    while( 1 )
    {
        while( xSemaphoreTake( xSema_Handle, portMAX_DELAY ) == pdTRUE )
        {
            xReceive_status = xQueueReceive( xData_QueueHandle, &xData_received, portMAX_DELAY );
            if( xReceive_status == pdTRUE )
            {
                char buff[15];
                sprintf( buff, "Consumer2: %lu\n", xData_received );
                prvUART1_Write( buff );
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
