/**
 * \file        prv_functions.c
 * \author      Marco, Roldan L.
 * \version     v1.0
 * \date        03/06/2021
 * \brief       collection of useful functions
 */


#include "prv_functions.h"

uint16_t map(uint16_t value, uint16_t fromLow, uint16_t fromHigh, uint16_t toLow, uint16_t toHigh) {
    
    return ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow;
}

double fmap(uint16_t value, uint16_t fromLow, uint16_t fromHigh, double toLow, double toHigh) {
    
    return ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow;
}
