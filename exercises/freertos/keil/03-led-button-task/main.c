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
void led_task_handler(void *params);
void button_task_handler(void *params);


/* Globals */
uint8_t button_status_flag = 0;


int main(void) {
    
    prv_periph_init();
    
    // creating task
	xTaskCreate(led_task_handler, "led-task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(button_task_handler, "button-task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    
    // start the scheduler
	vTaskStartScheduler();
	for(;;);
}


void led_task_handler(void *params) {
    
	while(1) {
        if(button_status_flag == 1) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
        }
        else {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
        }
    }  
}

void button_task_handler(void *params) {
    
	while(1) {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
            button_status_flag = 1;
        }
        else{
            button_status_flag = 0;
        }
    }
}
