#include "functions.h"


/**
* @brief    map a 16 bit range of values to desired range of values
* @params   value: original value to be mapped
*           fromLow: lowest original value
*           fromHigh: highest original value
*           toLow: lowest desired value
*           toHigh: highest desired value
* @retval   16 bits mapped value
*/

uint16_t map(uint16_t value, uint16_t fromLow, uint16_t fromHigh, uint16_t toLow, uint16_t toHigh) {
    
    return ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow;
    
}


/**
* @brief    map a double type range of values to desired range of values
* @params   value: original value to be mapped
*           fromLow: lowest original value
*           fromHigh: highest original value
*           toLow: lowest desired value
*           toHigh: highest desired value
* @retval   double type mapped value
*/

double fmap(uint16_t value, uint16_t fromLow, uint16_t fromHigh, double toLow, double toHigh) {
    
    return ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow;
    
}
