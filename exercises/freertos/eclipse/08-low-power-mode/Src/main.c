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
void vApplicationIdleHook( void );

/* Task Handles */
TaskHandle_t xTask1_TaskHandle = NULL;
TaskHandle_t xTask2_TaskHandle = NULL;

/* Globals */


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
        prvGPIO_ToggleBit( GPIOC, GPIO_Pin_13 );
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}


void vTask2_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        uint8_t ucBit_state = 0;
        ucBit_state = GPIO_ReadOutputDataBit( GPIOC, GPIO_Pin_13 );
        char bit[10];
        sprintf( bit, "%u", ucBit_state );
        prvUART1_Write(bit);
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}


void vApplicationIdleHook( void )
{
    __WFI(); /* put the processor in low power mode */
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
