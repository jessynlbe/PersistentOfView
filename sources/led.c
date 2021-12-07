#include "../headers/led.h"

void init_leds(){
    DDRB |= _BV(PB5); // SCK / CLK
    DDRB |= _BV(PB3); // MOSI / SDI
    DDRC |= _BV(PC1); // OE
    DDRC |= _BV(PC2); // LE
}

void turn_on_leds(){
    int idx = 0;
    while(idx < 16){

        PORTB |= _BV(PB3);
        PORTB |= _BV(PB5);
        PORTB &= ~_BV(PB3);
        PORTB &= ~_BV(PB5);
        
        idx += 1;

    }

    PORTC |= _BV(PC2);
    PORTC &= ~_BV(PC1);
}

void turn_off_leds(){
    int idx = 0;
    while(idx < 16){

        PORTB &= ~_BV(PB3);
        PORTB |= _BV(PB5);
        PORTB &= ~_BV(PB3);
        PORTB &= ~_BV(PB5);
        
        idx += 1;

    }

    PORTC |= _BV(PC2);
    PORTC &= ~_BV(PC1);
}