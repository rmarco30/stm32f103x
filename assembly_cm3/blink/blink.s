; RCC_BASE				0x40021000
; RCC_APB2ENR_OFFSET	0x18	;Bit 4 IOPCEN

; GPIOC_PORT_BASE		0x40011000
; GPIOx_CRH_OFFSET		0x04
; RESET_VALUE 			0x4444 4444
; <21:20> MODE13: 11
; <23:22> CNF13: 00

; GPIOx_BSRR_OFFSET		0x10
; <13>	BS13: 1
; <29>	BR13: 1

RCC_BASE    EQU 0x40021000
RCC_APB2ENR	EQU	0x40021018
GPIOC_CRH	EQU	0x40011004
GPIOC_ODR	EQU	0x4001100C
				
			AREA	|.text|, CODE, READONLY, ALIGN=2
			THUMB
			ENTRY
			EXPORT __main
				
__main

			BL	GPIOC_INIT
	
LOOP		
            BL	LIGHT_ON
			B	LOOP
	
	
GPIOC_INIT
	
            LDR R1, =RCC_BASE
            LDR R0, [R1, #0x18]
            ORR R0, R0, #0x10
            STR R0, [R1, #0x18]
            
            LDR R1, =GPIOC_CRH
            LDR R0, [R1]
            ORR R0, R0, #0x00300000
            AND R0, R0, #0xFF3FFFFF
            STR R0, [R1]

			BX	LR
			
LIGHT_ON
			LDR R1, =GPIOC_ODR
			LDR R0, [R1]
			ORR R0, R0, #0x1000
			STR R0, [R1]
			BX	LR
            
            ALIGN
            END
	