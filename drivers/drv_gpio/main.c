#include "stm32f10x.h"
#include "drv_gpio.h"


int main(void) {
	
    // create instance of structure initializer
    gpio_pin_conf_t gpio_pin_output;
    
    // populate the structure with the desired port operation
    gpio_pin_output.mode = GPIO_MODE_OUTPUT_50MHZ;
    gpio_pin_output.cnf = GPIO_CNF_OUTPUT_PP;
    
    // enable GPIOA clock
    gpio_clk_en(GPIOA_CLK);
    
    // initialize the gpio to be used
    gpio_init(GPIOA, 0, &gpio_pin_output);
    gpio_init(GPIOA, 1, &gpio_pin_output);
    gpio_init(GPIOA, 2, &gpio_pin_output);
    gpio_init(GPIOA, 3, &gpio_pin_output);
    gpio_init(GPIOA, 4, &gpio_pin_output);
    
    
    // create instance of structure initializer
    gpio_pin_conf_t gpio_pin_input;
    
    // populate the structure with the desired port operation
    gpio_pin_input.mode = GPIO_MODE_INPUT;
    gpio_pin_input.cnf = GPIO_CNF_INPUT_FLOAT;
    
    // enable GPIOB clock
    gpio_clk_en(GPIOB_CLK);
    
    // initialize the gpio to be used
    gpio_init(GPIOB, 8, &gpio_pin_input);
    
    while(1) {
        
    }
}
