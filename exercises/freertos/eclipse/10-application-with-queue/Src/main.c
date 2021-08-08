/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    Aug 6, 2021
  * @brief   One task will send data into queue and another is waiting for data
  *          in the queue.
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
#define EXIT_APP            ( 0 )

/* Function Prototypes */
void vMenuDisplay_TaskHandler( void *pvParam );
void vCmdHandling_TaskHandler( void *pvParam );
void vCmdProcessing_TaskHandler( void *pvParam );
void vUartWrite_TaskHandler( void *pvParam );
void USART1_IRQHandler( void );

void vTurnLedOn( char *task_msg );
void vLed_toggle(TimerHandle_t xTimer);
void vTurnLedOff( char *task_msg );
void vTurnLedToggleOn( uint32_t duration, char *task_msg );
void vTurnLedToggleOff( char *task_msg );
void vReadLedStatus( char *task_msg );
void vReadRTC( char *task_msg );
void vExitApp(char *task_msg);
void vPrintErrorMsg( char *task_msg );

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

/* Timer Handles */
static TimerHandle_t xLedTimer_Handle = NULL;


/* Globals */
static uint8_t ucCommand_buffer[20];
static uint8_t ucCommand_len = 0;

/* structure for receiving commands and arguments */
typedef struct App_Cmd {
    uint8_t Command_Num;
    uint8_t Command_Args[10];
} App_Cmd_t;

/* Application menu */
char menu[] = {"\n           \
    \nLED_ON            -> 1 \
    \nLED_OFF           -> 2 \
    \nLED_TOGGLE        -> 3 \
    \nLED_TOGGLE_OFF    -> 4 \
    \nLED_READ_STATUS   -> 5 \
    \nRTC_PRINT         -> 6 \
    \nEXIT_APP          -> 0 \
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

        taskENTER_CRITICAL();                                       /* race condition guard, disable interrupts */
        ucCmd_code = ucGetCmdCode( ucCommand_buffer );              /* convert the command (ascii) from user to number via helper function */
        xNew_cmd->Command_Num = ucCmd_code;                         /* save the command code */
        vGetCmdArgs( xNew_cmd->Command_Args );                      /* not implemented */
        taskEXIT_CRITICAL();                                        /* race condition guard, enable interrupts */

        /* send command to command queue */
        xQueueSend( xCommand_Queue, &xNew_cmd, portMAX_DELAY );     /* send the new command to queue */
        vPortFree(xNew_cmd);                                        /* free the allocated memory to xNew_cmd */
    }
}


void vCmdProcessing_TaskHandler( void *pvParam )
{
    App_Cmd_t *xCmd;
    char task_msg[50];
    uint32_t toggle_duration = pdMS_TO_TICKS(500);

    while( 1 )
    {
        xQueueReceive( xCommand_Queue, (void*)&xCmd, portMAX_DELAY );

        if(xCmd->Command_Num == LED_ON_CMD)
        {
            vTurnLedOn( task_msg );
        }
        else if( xCmd->Command_Num == LED_OFF_CMD )
        {
            vTurnLedOff( task_msg );
        }
        else if( xCmd->Command_Num == LED_TOGGLE_ON_CMD )
        {
            vTurnLedToggleOn( toggle_duration, task_msg);
        }
        else if( xCmd->Command_Num == LED_TOGGLE_OFF_CMD )
        {
            vTurnLedToggleOff( task_msg );
        }
        else if( xCmd->Command_Num == LED_READ_CMD )
        {
            vReadLedStatus( task_msg );
        }
        else if( xCmd->Command_Num == RTC_READ_CMD )
        {
            vReadRTC( task_msg );
        }
        else if( xCmd->Command_Num == EXIT_APP )
        {
            vExitApp( task_msg );
        }
        else
        {
            vPrintErrorMsg( task_msg );
        }
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
 * @brief	Notify the CmdHandling and MenuDisplay task that the user enters a command
 * @param	none
 * @retval	none
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
 * @brief	helper function that converts the command ascii code to number
 * @param	buffer: expects the address of the buffer where ascii to be converted is stored
 * @retval	none
 */
uint8_t ucGetCmdCode( uint8_t *buffer )
{
    /* convert ascii to number */
    return buffer[0] - 48;
}


/**
 * @brief	feature not implemented
 * @param	feature not implemented
 * @retval	feature not implemented
 */
void vGetCmdArgs( uint8_t *buffer )
{
    /* feature not implemented */
}


/**
 * @brief	Turn on the on-board led connected on PC13
 * @param   task_msg: pointer to buffer where message is stored
 * @retval	none
 */
void vTurnLedOn( char *task_msg )
{
    /* checks if toggle_timer was created, if yes then stop it and turn on the led */
    if( xLedTimer_Handle != NULL )
    {
        /* stop the timer */
        xTimerStop( xLedTimer_Handle, portMAX_DELAY );
    }
    GPIO_WriteBit( GPIOC, GPIO_Pin_13, Bit_RESET );
}


/**
 * @brief   Turn off the on-board led connected on PC13
 * @param   task_msg: pointer to buffer where message is stored
 * @retval  none
 */
void vTurnLedOff( char *task_msg )
{
    /* checks if toggle_timer was created, if yes then stop it and turn off the led */
    if( xLedTimer_Handle != NULL )
    {
        /* stop the timer */
        xTimerStop( xLedTimer_Handle, portMAX_DELAY );
    }
    GPIO_WriteBit( GPIOC, GPIO_Pin_13, Bit_SET );
}


/**
 * @brief   Creates a RTOS timer that will periodically toggle the on-board led connected on PC13
 * @param   duration: toggle duration in milliseconds
 * @param   task_msg: pointer to buffer where message is stored
 * @retval  none
 */
void vTurnLedToggleOn( uint32_t duration, char *task_msg )
{
    if(xLedTimer_Handle == NULL)
    {
        /* create a timer that will be periodically called to toggle the LED */
        xLedTimer_Handle = xTimerCreate( "toggle_timer", duration, pdTRUE, NULL, &vLed_toggle );
    }
    else
    {
        task_msg = "Led is already toggling, why bother?";
        xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
    }
    /* start the timer */
    xTimerStart( xLedTimer_Handle, portMAX_DELAY );
}


/**
 * @brief	Helper function parameter to xTimerCreate() API, this is responsible for toggling the led
 * @param	xTimer: not used. Required by the timer callback fuction prototype.
 * @retval	none
 */
void vLed_toggle(TimerHandle_t xTimer)
{
    prvGPIO_ToggleBit(GPIOC, GPIO_Pin_13);
}


/**
 * @brief	Turns off led toggling
 * @param   task_msg: pointer to buffer where message is stored
 * @retval	none
 */
void vTurnLedToggleOff( char *task_msg )
{
    /* checks if toggle_timer was created, if yes then stop it */
    if( xLedTimer_Handle != NULL )
    {
        /* stop the timer */
        xTimerStop( xLedTimer_Handle, portMAX_DELAY );
    }
    else
    {
        task_msg = "Led is not toggling, why stop it?";
        xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
    }
}


/**
 * @brief	Reads the current state of the on-board led
 * @param	task_msg: a pointer to a buffer where message is stored
 * @retval	none
 */
void vReadLedStatus(char *task_msg)
{
    uint8_t led_state = GPIO_ReadOutputDataBit( GPIOC, GPIO_Pin_13 );
    led_state ^= 1;
    sprintf( task_msg, "Led Status is: %d", led_state);
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}


/**
 * @brief	Reads the RTC registers to display time and date
 * @param	task_msg: a pointer to a buffer where message is stored
 * @retval	none
 */
void vReadRTC( char *task_msg )
{
    task_msg = "RTC is not yet configured!";
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}


/**
 * @brief	Exits the application
 * @param	task_msg: a pointer to a buffer where message is stored
 * @retval	none
 */
void vExitApp(char *task_msg)
{
    task_msg = "Feature not implemented";
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}


/**
 * @brief   Prints the error message for not recognized commands
 * @param	task_msg: a pointer to a buffer where message is stored
 * @retval  none
 */
void vPrintErrorMsg(char *task_msg)
{
    sprintf( task_msg, "Command Error!" );
    xQueueSend( xUartWrite_Queue, &task_msg, portMAX_DELAY );
}


/**
 * @brief	Blocking delay that waits in milliseconds
 * @param	ulTicks_in_ms: time to wait in milliseconds
 * @retval	none
 */
void vBlocking_delay( uint32_t ulTicks_in_ms )
{
    uint32_t ulCurrent_tick_count = xTaskGetTickCount();
    uint32_t ulDelay_in_ticks = ( ulTicks_in_ms * configTICK_RATE_HZ ) / 1000;
    while( xTaskGetTickCount() < ( ulCurrent_tick_count + ulDelay_in_ticks ) );
}
