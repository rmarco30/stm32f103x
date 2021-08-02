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
void vTask1_TaskHandler( void *pvParam );
void vTask2_TaskHandler( void *pvParam );
void vRTOS_delay( uint32_t ulTicks_in_ms );
void GPIO_ToggleBit( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin );
void vApplicationIdleHook( void );


/* Globals */


/* Task Handles */
static TaskHandle_t xTask1_TaskHandle = NULL;
static TaskHandle_t xTask2_TaskHandle = NULL;


int main( void )
{
    /* initialize private peripherals (GPIOs, UART, etc) */
    prvPeriph_Init();
    
    /* tasks */
	xTaskCreate( vTask1_TaskHandler, "task-1", 1000, NULL, 2, &xTask1_TaskHandle );
    xTaskCreate( vTask2_TaskHandler, "task-2", 1000, NULL, 3, &xTask2_TaskHandle );
    
    /* start the scheduler */
	vTaskStartScheduler();
    
	for( ;; );
}

void vTask1_TaskHandler( void *pvParam )
{
    while( 1 )
    {
        GPIO_ToggleBit( GPIOC, GPIO_Pin_13 );
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

/**
 @name   GPIO_ToggleBit
 @brief  Toggles the selected GPIO output pin
 @param  GPIOx: Any of the GPIO_TypeDef* available (GPIOA, GPIOB, GPIOC, ..)
 @param  GPIO_Pin: Any of the GPIO_Pin_x where x is the pin number
 @retval none
*/
void GPIO_ToggleBit( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin )
{
    uint8_t ucPin_value = 0;
    ucPin_value = GPIO_ReadOutputDataBit( GPIOx, GPIO_Pin );
    ucPin_value ^= 1;
    
    GPIO_WriteBit( GPIOx, GPIO_Pin, (BitAction)ucPin_value );
}




//void vApplicationIdleHook( void )
//{
//    __WFI();
//}
