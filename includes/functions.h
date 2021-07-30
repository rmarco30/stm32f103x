#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "stdint.h"

uint16_t map(uint16_t value, uint16_t fromLow, uint16_t fromHigh, uint16_t toLow, uint16_t toHigh);

double fmap(uint16_t value, uint16_t fromLow, uint16_t fromHigh, double toLow, double toHigh);

#endif // __FUNCTIONS_H
