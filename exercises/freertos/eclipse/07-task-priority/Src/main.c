/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 6, 2021
  * @brief   Tasks will switch priority on button press
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
void vRTOS_delay( uint32_t ulTicks_in_ms );

/* Task Handlers */
void vTask1_TaskHandler( void *pvParam );
void vTask2_TaskHandler( void *pvParam );

/* Task Handles */
TaskHandle_t xTask1_TaskHandle = NULL;
TaskHandle_t xTask2_TaskHandle = NULL;

/* Globals */
char prio_num[50];
UBaseType_t uxPriority_number[2];

int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();

    /* tasks */
    xTaskCreate( vTask1_TaskHandler, "task-1", 1000, NULL, 2, &xTask1_TaskHandle );
    xTaskCreate( vTask2_TaskHandler, "task-2", 1000, NULL, 3, &xTask2_TaskHandle );

    /* start the scheduler */
    vTaskStartScheduler();


    /* program should not reach here */
    for( ;; );
}

void vTask1_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        vRTOS_delay( 200 );
        prvGPIO_ToggleBit( GPIOC, GPIO_Pin_13 );
        uxPriority_number[0] = uxTaskPriorityGet( xTask1_TaskHandle );
        uxPriority_number[1] = uxTaskPriorityGet( xTask2_TaskHandle );
        sprintf( prio_num, "task1 priority: %lu , task2 priority: %lu\n", uxPriority_number[0], uxPriority_number[1] );
        prvUART1_Write( prio_num );
    }
}


void vTask2_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        if( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_0 ) == 0 )
        {
            vRTOS_delay(150);
            uxPriority_number[0] = uxTaskPriorityGet( xTask1_TaskHandle );
            uxPriority_number[1] = uxTaskPriorityGet( xTask2_TaskHandle );
            prvUART1_Write( "task priority switched from task-2\n" );
            vTaskPrioritySet( xTask1_TaskHandle, uxPriority_number[1] );
            vTaskPrioritySet( xTask2_TaskHandle, uxPriority_number[0] );
        }
        else
        {
            vRTOS_delay( 1000 );
            prvGPIO_ToggleBit( GPIOC, GPIO_Pin_13 );
            uxPriority_number[0] = uxTaskPriorityGet( xTask1_TaskHandle );
            uxPriority_number[1] = uxTaskPriorityGet( xTask2_TaskHandle );
            sprintf( prio_num, "task1 priority: %lu , task2 priority: %lu\n", uxPriority_number[0], uxPriority_number[1] );
            prvUART1_Write( prio_num );
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
