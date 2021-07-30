/**
 * \file        drv_gpio.c
 * \author      Marco, Roldan L.
 * \version     V1.0
 * \date        06/06/2021
 * \brief       stm32f103x gpio driver to configure ports
 */

#ifndef DRV_GPIO_H
#define DRV_GPIO_H

/* Includes --------------------------------------------*/
#include "stm32f10x.h"
#include <stdint.h>

/* Defines ---------------------------------------------*/

/* Choose MODE (INPUT or OUTPUT) */
#define GPIO_MODE_INPUT             ((uint32_t)0x00)
#define GPIO_MODE_OUTPUT_10MHZ      ((uint32_t)0x01)
#define GPIO_MODE_OUTPUT_2MHZ       ((uint32_t)0x02)
#define GPIO_MODE_OUTPUT_50MHZ      ((uint32_t)0x03)

/* Choose CONFIGURATION for INPUT mode */
#define GPIO_CNF_INPUT_ANALOG       ((uint32_t)0x00)
#define GPIO_CNF_INPUT_FLOAT        ((uint32_t)0x01)
#define GPIO_CNF_INPUT_PUPD         ((uint32_t)0x02)

/* Choose CONFIGURATION for OUTPUT mode */
#define GPIO_CNF_OUTPUT_PP          ((uint32_t)0x00)
#define GPIO_CNF_OUTPUT_OD          ((uint32_t)0x01)

/* Choose CONFIGURATION for ALTERNATE FUNCTION */
#define GPIO_CNF_OUTPUT_ALTF_PP     ((uint32_t)0x02)
#define GPIO_CNF_OUTPUT_ALTF_OD     ((uint32_t)0x03)

/* Choose which GPIO CLOCK/s will be enabled */
#define GPIOA_CLK                   (RCC_APB2ENR_IOPAEN)
#define GPIOB_CLK                   (RCC_APB2ENR_IOPBEN)
#define GPIOC_CLK                   (RCC_APB2ENR_IOPCEN)
#define GPIOD_CLK                   (RCC_APB2ENR_IOPDEN)

/* Structs --------------------------------------------*/
typedef struct {
    uint32_t mode;
    uint32_t cnf;
} gpio_pin_conf_t;

/* Function Prototypes --------------------------------*/

/**
 * \name   - gpio_clk_en
 * \brief  - enables the selected gpio clock in the RCC register
 * \param  - gpiox_clk : expects one of the GPIOX_CLK defines
 * \retval - none
 */
void gpio_clk_en (uint32_t gpiox_clk);


/**
 * \name   - gpio_clk_en
 * \brief  - enables the selected gpio clock in the RCC register
 * \param  - GPIOx : expects the address of one of the GPIO available (GPIOA, GPIOB, GPIOC, etc)
 * \param  - pin : expects the pin/port number
 * \param  - gpio_pin_conf : expects the address of initializer variable with the type gpio_pin_conf_t
 * \retval - none
 */
void gpio_init(GPIO_TypeDef *GPIOx, uint8_t pin, gpio_pin_conf_t *gpio_pin_conf);

#endif
