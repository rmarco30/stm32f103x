#include "stm32f10x.h"
#include "drv_gpio.h"


int main(void) {
	
    gpio_pin_conf_t gpio_pin_output;
    gpio_pin_output.mode = GPIO_MODE_OUTPUT_50MHZ;
    gpio_pin_output.cnf = GPIO_CNF_OUTPUT_PP;
    
    gpio_clk_en(GPIOA_CLK);
    gpio_init(GPIOA, 0, &gpio_pin_output);
    gpio_init(GPIOA, 1, &gpio_pin_output);
    gpio_init(GPIOA, 2, &gpio_pin_output);
    gpio_init(GPIOA, 3, &gpio_pin_output);
    gpio_init(GPIOA, 4, &gpio_pin_output);
    
    gpio_pin_conf_t gpio_pin_input;
    gpio_pin_input.mode = GPIO_MODE_INPUT;
    gpio_pin_input.cnf = GPIO_CNF_INPUT_FLOAT;
    
    gpio_init(GPIOB, 8, &gpio_pin_input);
    
    while(1) {
        
    }
}
