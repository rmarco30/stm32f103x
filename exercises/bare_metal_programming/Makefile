CC = arm-none-eabi-gcc
MACH = cortex-m3
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=final.map

all : main.o gpio.o stm32_startup.o final.elf

load:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

main.o : main.c
	$(CC) $(CFLAGS) -o $@ $^

gpio.o : gpio.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o : stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf : main.o gpio.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	rm -rf *.o *.elf