/**
 * \file        drv_gpio.c
 * \author      Marco, Roldan L.
 * \version     V1.0
 * \date        06/06/2021
 * \brief       stm32f103x gpio driver to configure ports
 */

#include "stm32f10x.h"
#include "drv_gpio.h"



void gpio_clk_en (uint32_t gpiox_clk) {

    RCC->APB2ENR |= gpiox_clk;
}

void gpio_init(GPIO_TypeDef *GPIOx, uint8_t pin, gpio_pin_conf_t *gpio_pin_conf) {

    if (pin < 8) {
        
        /* clear pin fields */
        GPIOx->CRL &= ~((uint32_t)0x0f << (pin * 4));

        /* set pin mode */
        GPIOx->CRL |= gpio_pin_conf->mode << (pin * 4);

        /* set pin configuration */
        GPIOx->CRL |= (gpio_pin_conf->cnf << 2) << (pin * 4);
    }
    
    else {
        
        pin = pin % 8;
        
        /* clear pin fields */
        GPIOx->CRH &= ~((uint32_t)0x0f << (pin * 4));

        /* set pin mode */
        GPIOx->CRH |= gpio_pin_conf->mode << (pin * 4);

        /* set pin configuration */
        GPIOx->CRH |= (gpio_pin_conf->cnf << 2) << (pin * 4);
    }
}
