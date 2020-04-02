## Makefile for the TWI module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: twi.o uart.o debug.o tim16.o mlx90614.o app app_hex

twi.o: twi.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c twi.c

uart.o: uart.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c uart.c

debug.o: debug.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c debug.c

tim16.o: tim16.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c tim16.c

mlx90614.o: mlx90614.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c mlx90614.c

app: twi.o uart.o debug.o tim16.o mlx90614.o twi.h uart.h debug.h tim16.h mlx90614.h
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o app twi.o uart.o debug.o tim16.o mlx90614.o main.c
	
app_hex:
	avr-objcopy -O ihex -R .eeprom app app.hex

clean:
	rm *.hex *.o app
