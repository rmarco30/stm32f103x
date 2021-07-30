/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


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
    
	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);
	vTaskStartScheduler();
	for(;;);
}


void vTask1_handler(void *params) {
	while(1) {
        if(UART_ACCESS_KEY == AVAILABLE) {
            UART_ACCESS_KEY = NON_AVAILABLE;
            uart1_tx("hello from task 1\n");
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
            uart1_tx("hello from task 2\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            UART_ACCESS_KEY = AVAILABLE;
            taskYIELD();
        }
    }
}