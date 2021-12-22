CFLAGS=-Os -DF_CPU=13000000 -mmcu=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
MCU=m328p

spi.o: spi.c
	${CC} ${CFLAGS} -c -o spi.o spi.c

timer.o: timer.c
	${CC} ${CFLAGS} -c -o timer.o timer.c -lm

usart.o: usart.c
	${CC} ${CFLAGS} -c -o usart.o usart.c

magnetic.o: magnetic.c
	${CC} ${CFLAGS} -c -o magnetic.o magnetic.c

main.out: main.c magnetic.o usart.o timer.o spi.o
	${CC} ${CFLAGS} -o main.out main.c magnetic.o usart.o timer.o spi.o -lm

main.bin: main.out
		${OBJCOPY} -O binary main.out main.bin

all: main.bin
	avrdude -c usbasp -p ${MCU} -U flash:w:main.bin

clean:
		rm *.o *.out *.bin
