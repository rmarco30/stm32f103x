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


/* Function Prototypes */
void vTask1_handler(void *params);
void vTask2_handler(void *params);


/* Globals */
TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;


int main(void) {

    // creating task
	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

    // start the scheduler
	vTaskStartScheduler();
	for(;;);
}


void vTask1_handler(void *params) {

	while(1);
}

void vTask2_handler(void *params) {

	while(1);
}
