#include "../headers/spi.h"
#include "../headers/usart.h"

void init_spi(){
    DDRB |= _BV(PB2); // SS
    DDRB |= _BV(PB3); // MOSI / SDI
    DDRB |= _BV(PB5); // SCK / CLK

    SPCR |= _BV(MSTR); // Master SPI Mode
    SPCR |= _BV(SPE); // Enable SPI

    DDRC |= _BV(PC1); // OE
    DDRC |= _BV(PC2); // LE

    led_on = 0;
}

void spi_transmit(uint8_t first , uint8_t second){
    SPDR = first;
    while( !(SPSR & _BV(SPIF)) ); // Wait for the byte to be written

    SPDR = second;
    while( !(SPSR & _BV(SPIF)) ); // Wait for the byte to be written

    PORTC |= _BV(PC2);  // Toggle the latch pin to write both bytes to the LED driver memory
    PORTC &= ~_BV(PC2);
}

void toggle_leds(){
    if(led_on == 0){
        spi_transmit( (uint8_t) 255, (uint8_t) 255);
        led_on = 1;
    }
    else{
        spi_transmit( (uint8_t) 0, (uint8_t) 0);
        led_on = 0;
    }
}