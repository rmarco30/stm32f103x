#ifndef _PRV_PERIPH_H
#define _PRV_PERIPH_H

#include "stm32f10x.h"


#define TRUE            1
#define FALSE           0
#define AVAILABLE       TRUE
#define NON_AVAILABLE   FALSE

void prvPeriph_Init( void );
static void prvUART1_Init( void );
void prvUART1_Write( char *msg );
static void prvGPIO_Init( void );

#endif
