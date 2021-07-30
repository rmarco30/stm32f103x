#define RCC_BASE        0x40021000U
#define RCC_APB2ENR     (*((volatile unsigned int *)(RCC_BASE + 0x18U)))

#define GPIO_BASE      	0x40010C00U
#define GPIOB_CRL       (*((volatile unsigned int *)(GPIO_BASE)))
#define GPIOB_CRH       (*((volatile unsigned int *)(GPIO_BASE + 0x04)))
#define GPIOB_ODR       (*((volatile unsigned int *)(GPIO_BASE + 0x0CU)))

#define LED_RED         (1U << 0)
#define LED_GREEN       (1U << 1)
#define LED_ORANGE      (1U << 10)

int main() {
    
    RCC_APB2ENR = (1 << 3);                                            // enable clock for GPIOB
    GPIOB_CRL = (1 << 0) | (1 << 1) | (1 << 4) | (1 << 5);             // configure port B0 and B1 as output
    GPIOB_CRH = (1 << 8) | (1 << 9);                                   // configure port B10 as output
    
    GPIOB_ODR = LED_GREEN;
    
    while (1) {
      
        GPIOB_ODR |= LED_ORANGE;            // GPIOB_ODR
        
        int volatile counter = 0;
        while (counter < 500000) {
            ++counter;
        }
        
        GPIOB_ODR &= ~LED_ORANGE;            // GPIOB_ODR
        
        counter = 0;
        while (counter < 500000) {
            ++counter;
        }
    }

    return 0;
}
