/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 03, 2021
  * @brief   freertos using keil test code
  ******************************************************************************
*/

/* Includes */
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "prvHardwareSetup.h"
#include "prvHardwareOperations.h"

#include <stdio.h>

/* Application Command Defines */
#define LED_ON_CMD          ( 1 )
#define LED_OFF_CMD         ( 2 )
#define LED_TOGGLE_ON_CMD   ( 3 )
#define LED_TOGGLE_OFF_CMD  ( 4 )
#define LED_READ_CMD        ( 5 )
#define RTC_READ_CMD        ( 6 )

/* Function Prototypes */
void vMenuDisplay_TaskHandler( void *pvParam );
void vCmdHandling_TaskHandler( void *pvParam );
void vCmdProcessing_TaskHandler( void *pvParam );
void vUartWrite_TaskHandler( void *pvParam );
void USART1_IRQHandler(void);

void vTurnLedOn(void);
void vTurnLedOff(void);
void vTurnLedToggleOn(void);
void vTurnLedToggleOff(void);
void vReadLedStatus(char *task_msg);
void vReadRTC(char *task_msg);
void vPrintErrorMsg(char *task_msg);

uint8_t ucGetCmdCode( uint8_t *buffer );
void vGetCmdArgs( uint8_t *buffer );
void vBlocking_delay( uint32_t ulTicks_in_ms );


/* Task Handles */
static TaskHandle_t xMenuDisplay_Handle = NULL;
static TaskHandle_t xCmdHandling_Handle = NULL;
static TaskHandle_t xCmdProcessing_Handle = NULL;
static TaskHandle_t xUartWrite_Handle = NULL;


/* Queue Handles */
static QueueHandle_t xCommand_Queue;
static QueueHandle_t xUartWrite_Queue;


/* Globals */
static uint8_t ucCommand_buffer[20];
static uint8_t ucCommand_len = 0;

typedef struct App_Cmd {
    uint8_t Command_Num;
    uint8_t Command_Args[10];
} App_Cmd_t;

char menu[] = {"\n           \
    \nLED_ON            -> 1 \
    \nLED_OFF           -> 2 \
    \nLED_TOGGLE        -> 3 \
    \nLED_TOGGLE_OFF    -> 4 \
    \nLED_READ_STATUS   -> 5 \
    \nRTC_PRINT         -> 6 \
    \nEXIT_APP          -> 7 \
    \nSelect: "};


int main( void )
{
    /* initialize peripherals (GPIOs, UART, etc) */
    prvHardwareSetup();
    
    /* queues */
    xCommand_Queue = xQueueCreate( 10, sizeof(App_Cmd_t *) );
    xUartWrite_Queue = xQueueCreate( 10, sizeof(char *) );
    
    if( (xCommand_Queue == NULL) || (xUartWrite_Queue == NULL) )
    {
        return 0;
    }
    
    /* tasks */
	xTaskCreate( vMenuDisplay_TaskHandler, "Menu", 500, NULL, 1, &xMenuDisplay_Handle );
    xTaskCreate( vCmdHandling_TaskHandler, "CmdHandling", 500, NULL, 2, &xCmdHandling_Handle );
    xTaskCreate( vCmdProcessing_TaskHandler, "CmdProcessing", 500, NULL, 2, &xCmdProcessing_Handle );
    xTaskCreate( vUartWrite_TaskHandler, "UartWrite", 500, NULL, 2, &xUartWrite_Handle );
    
    
    /* start the scheduler */
	vTaskStartScheduler();
    
    /* program should not reach here */
	for( ;; );
}

void vMenuDisplay_TaskHandler( void *pvParam )
{
    char *pMenu = menu;
    while( 1 )
    {
        xQueueSend( xUartWrite_Queue, &pMenu, portMAX_DELAY );
        
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY );
    }
}


void vCmdHandling_TaskHandler( void *pvParam )
{
    uint8_t ucCmd_code = 0;
    App_Cmd_t *xNew_cmd;
    while( 1 )
    {
        xTaskNotifyWait( 0, 0, NULL, portMAX_DELAY );
        
        xNew_cmd = (App_Cmd_t *) pvPortMalloc( sizeof(App_Cmd_t) );
        
        taskENTER_CRITICAL();
        ucCmd_code = ucGetCmdCode( ucCommand_buffer );
        xNew_cmd->Command_Num = ucCmd_code;
        vGetCmdArgs( xNew_cmd->Command_Args );
        taskEXIT_CRITICAL();
        
        /* send command to command queue */
        xQueueSend( xCommand_Queue, &xNew_cmd, portMAX_DELAY );
    }
}


void vCmdProcessing_TaskHandler( void *pvParam )
{
    App_Cmd_t *xCmd;
    char task_msg[50];
    
    while( 1 )
    {
        xQueueReceive( xCommand_Queue, (void*)&xCmd, portMAX_DELAY );
        
        if(xCmd->Command_Num == LED_ON_CMD)
        {
            vTurnLedOn();
        }
        else if( xCmd->Command_Num == LED_OFF_CMD )
        {
            vTurnLedOff();
        }
        else if( xCmd->Command_Num == LED_TOGGLE_ON_CMD )
        {
            vTurnLedToggleOn();
        }
        else if( xCmd->Command_Num == LED_TOGGLE_OFF_CMD )
        {
            vTurnLedToggleOff();
        }
        else if( xCmd->Command_Num == LED_READ_CMD )
        {
            vReadLedStatus(task_msg);
        }
        else if( xCmd->Command_Num == RTC_READ_CMD )
        {
            vReadRTC(task_msg);
        }
        else
        {
            vPrintErrorMsg(task_msg);
        }
        
        vPortFree(xCmd);
    }
}


void vUartWrite_TaskHandler( void *pvParam )
{
    char *pData = NULL;
    while( 1 )
    {
        xQueueReceive( xUartWrite_Queue, &pData, portMAX_DELAY );
        prvUART1_Write( pData );
    }
}


/**
 @name   USART1_IRQHandler
 @brief  
 @param  
 @retval 
*/
void USART1_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint16_t usData_byte;
    if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) )
    {
        /* data byte received */
        usData_byte = USART_ReceiveData(USART1);
        ucCommand_buffer[ucCommand_len++] = usData_byte & 0xff;
        
        if(usData_byte == '\r')
        {
            ucCommand_len = 0;
            
            /* enter key pressed */
            xTaskNotifyFromISR(xCmdHandling_Handle, 0, eNoAction, &xHigherPriorityTaskWoken);
            xTaskNotifyFromISR(xMenuDisplay_Handle, 0, eNoAction, &xHigherPriorityTaskWoken);
        }
    }
    /* if any higher priority task wakes up from notification, then yield the processor to it */
    if( xHigherPriorityTaskWoken )
    {
        taskYIELD();
    }
}

/**
 @name   
 @brief  
 @param  
 @retval 
*/
uint8_t ucGetCmdCode( uint8_t *buffer )
{
    /* convert ascii to number */
    return buffer[0] - 48;
}



/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vGetCmdArgs( uint8_t *buffer )
{
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vTurnLedOn(void)
{
    GPIO_WriteBit( GPIOC, GPIO_Pin_13, Bit_RESET );
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vTurnLedOff(void)
{
    GPIO_WriteBit( GPIOC, GPIO_Pin_13, Bit_SET );
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vTurnLedToggleOn(void)
{
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vTurnLedToggleOff(void)
{
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vReadLedStatus(char *task_msg)
{
    uint8_t led_state = GPIO_ReadOutputDataBit( GPIOC, GPIO_Pin_13 );
    led_state ^= 1;
    sprintf( task_msg, "Led Status is: %d", led_state);
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vReadRTC(char *task_msg)
{
}


/**
 @name   
 @brief  
 @param  
 @retval 
*/
void vPrintErrorMsg(char *task_msg)
{
    sprintf( task_msg, "Command Error!" );
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}



/**
 @name   vBlocking_delay
 @brief  blocking delay that waits in milliseconds
 @param  ulTicks_in_ms - time to wait in milliseconds
 @retval none
*/
void vBlocking_delay( uint32_t ulTicks_in_ms )
{
    uint32_t ulCurrent_tick_count = xTaskGetTickCount();
    uint32_t ulDelay_in_ticks = ( ulTicks_in_ms * configTICK_RATE_HZ ) / 1000;
    while( xTaskGetTickCount() < ( ulCurrent_tick_count + ulDelay_in_ticks ) );
}
