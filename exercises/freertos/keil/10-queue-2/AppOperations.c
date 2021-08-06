#include "AppOperations.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"


/* extern */

extern QueueHandle_t xUartWrite_Queue;