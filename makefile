make: main2.c
	avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -Os main2.c
	avr-objcopy -O binary a.out a.bin
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -U flash:w:a.bin

https://prod.liveshare.vsengsaas.visualstudio.com/join?FDA86A5D8DBA18028C71F64C209A2D651511