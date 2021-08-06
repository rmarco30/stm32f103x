/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 6, 2021
  * @brief   
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"

/* Defines */


/* Function Prototypes */
void xLed_Handler(void *params);
void xButton_Handler(void *params);


/* Globals */
TaskHandle_t xLed_TaskHandle = NULL;
TaskHandle_t xButton_TaskHandle = NULL;
uint8_t button_status_flag = 0;


int main(void) {

	prvHardwareSetup();

    // creating task
	xTaskCreate(xLed_Handler, "led-task", configMINIMAL_STACK_SIZE, NULL, 2, &xLed_TaskHandle);
	xTaskCreate(xButton_Handler, "button-task", configMINIMAL_STACK_SIZE, NULL, 2, &xButton_TaskHandle);

    // start the scheduler
	vTaskStartScheduler();
	for(;;);
}


void xLed_Handler(void *params)
{

	while(1)
	{
	    if(button_status_flag == 1)
	    {
	        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	    }
	    else
	    {
	        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	    }
	}
}

void xButton_Handler(void *params)
{

	while(1)
	{
	    vTaskDelay(pdMS_TO_TICKS(100));
	    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
	    {

	        button_status_flag = 1;
	    }
	    else
	    {
	        button_status_flag = 0;
	    }
	}
}
