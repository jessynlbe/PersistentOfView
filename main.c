#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void main(void){
    DDRD |= _BV(PD6);
    DDRB |= _BV(PB5);

    DDRC |= _BV(PC1);

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
    
    PORTD |= _BV(PD6);

}