#ifndef _APP_OPERATIONS_H
#define _APP_OPERATIONS_H

#include "stm32f10x.h"

void vTurnLedOn(void);
void vTurnLedOff(void);
void vTurnLedToggleOn(void);
void vTurnLedToggleOff(void);
void vReadLedStatus(char *task_msg);
void vReadRTC(char *task_msg);
void vPrintErrorMsg(char *task_msg);

#endif /* _APP_OPERATIONS_H */
