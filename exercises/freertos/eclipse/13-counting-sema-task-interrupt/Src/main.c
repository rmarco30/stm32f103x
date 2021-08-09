/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 9, 2021
  * @brief   Synchronization of task and interrupt using binary semaphore.
  *          Demonstration of offloading ISR work to a helper function to reduce
  *          the time inside the ISR handler.
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"
#include <stdio.h>

/* Defines */


/* Function Prototypes */
void vRTOS_delay( uint32_t ulTicks_in_ms );

/* Task Handlers */
void vNormal_TaskHandler( void *pvParam );
void vHelper_TaskHandler( void *pvParam );

/* Task Handles */
TaskHandle_t xNormal_TaskHandle = NULL;
TaskHandle_t xHelper_TaskHandle = NULL;

/* Semaphores Handle*/
SemaphoreHandle_t xSema_Handle;

/* Globals */


int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* create tasks */
    // xTaskCreate( vNormal_TaskHandler, "normal-task", 500, NULL, 2, &xNormal_TaskHandle );
    xTaskCreate( vHelper_TaskHandler, "helper-task", 500, NULL, 3, &xHelper_TaskHandle );

    /* create semaphore */
    xSema_Handle = xSemaphoreCreateCounting( 10, 0 );

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


//void vNormal_TaskHandler( void *pvParam )
//{
//    while( 1 )
//    {
//        /* A normal running task */
//        prvUART1_Write( "Normal Running Task\n" );
//    }
//}


void vHelper_TaskHandler( void *pvParam )
{
    UBaseType_t uxSema_count;
    char cnt[20];

    while( 1 )
    {   /* will be blocked until a semaphore was released by ISR */
        while( xSemaphoreTake( xSema_Handle, portMAX_DELAY ) == pdTRUE )
        {
            uxSema_count = uxSemaphoreGetCount( xSema_Handle );
            sprintf( cnt, "Sema Count: %lu\n", uxSema_count);
            prvUART1_Write( cnt );
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

void EXTI0_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdTRUE;
    EXTI_ClearITPendingBit(EXTI_Line0);
    /* release 1 semaphore each time ISR runs */
    xSemaphoreGiveFromISR( xSema_Handle, &xHigherPriorityTaskWoken );
}
