#ifndef _PRV_PERIPH_H
#define _PRV_PERIPH_H

#include "stm32f10x.h"


#define TRUE            1
#define FALSE           0
#define AVAILABLE       TRUE
#define NON_AVAILABLE   FALSE

void prv_periph_init(void);
void uart1_init(void);
void uart1_tx(char *msg);
void gpio_init(void);

#endif
