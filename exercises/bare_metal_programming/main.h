#ifndef MAIN_H
#define MAIN_H


#define RCC_BASE        0x40021000U
#define RCC_APB2ENR     (*((volatile unsigned int *)(RCC_BASE + 0x18U)))

#define LED_RED         (1U << 0)
#define LED_GREEN       (1U << 1)
#define LED_ORANGE      (1U << 10)


void periph_clk_init(void);
void delay(int del);

#endif	// MAIN_H