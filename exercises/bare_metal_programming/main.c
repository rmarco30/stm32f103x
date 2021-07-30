#include "main.h"
#include "gpio.h"

uint32_t volatile x = 0xdeadbeef;
int y = 25;
int a, b, c;

const uint32_t my_variable1 = 100;
const uint32_t my_variable2 = 100;
const uint8_t my_variable3 = 100;

int main(void) {
    
    periph_clk_init();
    gpio_init();
    y++;
    a = 1;
    b = 2;
    c = a + b;
    
    while (1) {
      
        turn_on_led(LED_ORANGE);
        delay(100000); 
        turn_off_led(LED_ORANGE);
        delay(100000);
    }
}

void periph_clk_init(void) {

    // enable clock for GPIOB
    RCC_APB2ENR = (1 << 3);                 
}


void delay(int del) {
    int volatile counter = 0;
    while(counter < del) {
        ++counter;
    }
} 