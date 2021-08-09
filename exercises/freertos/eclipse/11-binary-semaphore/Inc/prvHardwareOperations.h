#ifndef _PRV_HARDWARE_OPERATIONS_H
#define _PRV_HARDWARE_OPERATIONS_H

#include "stm32f10x.h"

void prvUART1_Write( char *msg );
void prvGPIO_ToggleBit( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin );

#endif /* _PRV_HARDWARE_OPERATIONS_H */
