#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * \name   - map
 * \brief  - map a 16 bit range of values to desired 16 bit range of values
 * \param  - value : original value to be mapped
 * \param  - fromLow : lowest original value
 * \param  - fromHigh : highest original value
 * \param  - toLow : lowest desired value
 * \param  - toHigh : highest desired value
 * \retval - 16-bit mapped value
 */
uint16_t map(uint16_t value, uint16_t fromLow, uint16_t fromHigh, uint16_t toLow, uint16_t toHigh);


/**
 * \name   - fmap
 * \brief  - map a double type range of values to desired range of values
 * \param  - value : original value to be mapped
 * \param  - fromLow : lowest original value
 * \param  - fromHigh : highest original value
 * \param  - toLow : lowest desired value
 * \param  - toHigh : highest desired value
 * \retval - double type mapped value
 */
double fmap(uint16_t value, uint16_t fromLow, uint16_t fromHigh, double toLow, double toHigh);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __FUNCTIONS_H
