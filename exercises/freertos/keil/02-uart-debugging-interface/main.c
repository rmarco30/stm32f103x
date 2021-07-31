/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    June 01, 2021
  * @brief   freertos using keil test code
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "prv_periph.h"


/* Function Prototypes */
void vTask1_handler(void *params);
void vTask2_handler(void *params);


/* Globals */
TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;

uint8_t UART_ACCESS_KEY = AVAILABLE;


int main(void) {
    
    prv_periph_init();
    
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
            UART_ACCESS_KEY = NON_AVAILABLE;
            uart1_tx("hello 1\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            UART_ACCESS_KEY =  AVAILABLE;
            taskYIELD();
        }

    }  
}

void vTask2_handler(void *params) {
    
	while(1) {
        if(UART_ACCESS_KEY == AVAILABLE) {
            UART_ACCESS_KEY = NON_AVAILABLE;
            uart1_tx("hello 2\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            UART_ACCESS_KEY = AVAILABLE;
            taskYIELD();
        }
    }
}
