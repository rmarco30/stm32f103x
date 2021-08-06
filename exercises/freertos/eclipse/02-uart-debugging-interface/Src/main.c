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

#define AVAILABLE       ( 1 )
#define NOT_AVAILABLE   ( 0 )


/* Function Prototypes */
void vTask1_handler(void *params);
void vTask2_handler(void *params);


/* Globals */
TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;
uint8_t UART_ACCESS_KEY = AVAILABLE;


int main(void) {

	prvHardwareSetup();

    // creating task
	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

    // start the scheduler
	vTaskStartScheduler();
	for(;;);
}


void vTask1_handler(void *params) {

	while(1) {
	    if(UART_ACCESS_KEY == AVAILABLE) {
	        UART_ACCESS_KEY = NOT_AVAILABLE;
	        prvUART1_Write("hello from task 1\n");
	        vTaskDelay(pdMS_TO_TICKS(500));
	        UART_ACCESS_KEY = AVAILABLE;
	        taskYIELD();
	    }
	}
}

void vTask2_handler(void *params) {

	while(1) {
	    if(UART_ACCESS_KEY == AVAILABLE) {
	        UART_ACCESS_KEY = NOT_AVAILABLE;
	        prvUART1_Write("hello from task 2\n");
	        vTaskDelay(pdMS_TO_TICKS(500));
	        UART_ACCESS_KEY = AVAILABLE;
	        taskYIELD();
	    }
	}
}
