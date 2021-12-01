# avr-gcc -DF_CPU=16000000 -mmcu=atmega328p -Os main.c
#  avr-objcopy -O binary a.out a.bin
# avrdude -c arduino -p m328p -P /dev/ttyACM0 -U flash:w:a.bin

MCU=m328p
F_CPU=13000000
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -mmcu=atmega328p -DF_CPU=${F_CPU}
SRC=main
PORT=/dev/ttyACM0

all: 
	${CC}  ${CFLAGS} ${SRC}.c
	${OBJCOPY} -O binary a.out a.bin
	avrdude -c usbasp -p ${MCU} -P ${PORT} -U flash:w:a.bin