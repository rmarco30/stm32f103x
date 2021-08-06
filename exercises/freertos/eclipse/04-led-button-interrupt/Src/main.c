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
void button_handler(void);


/* Globals */
TaskHandle_t xLed_TaskHandle = NULL;
uint8_t button_status_flag = 0;


int main(void) {

	prvHardwareSetup();

    // creating task
	xTaskCreate(xLed_Handler, "led-task", configMINIMAL_STACK_SIZE, NULL, 2, &xLed_TaskHandle);

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

void button_handler()
{
    button_status_flag ^= 1;
}

void EXTI0_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0);
    button_handler();
}
