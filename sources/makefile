CFLAGS=-Os -DF_CPU=13000000 -mmcu=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
MCU=m328p

led.o: led.c
	${CC} ${CFLAGS} -c -o led.o led.c

magnetic.o: magnetic.c
	${CC} ${CFLAGS} -c -o magnetic.o magnetic.c

main.out: main.c magnetic.o led.o
	${CC} ${CFLAGS} -o main.out main.c magnetic.o led.o

main.bin: main.out
		${OBJCOPY} -O binary main.out main.bin

all: main.bin
	avrdude -c usbasp -p ${MCU} -U flash:w:main.bin

clean:
		rm *.o *.out *.bin
