#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void main(void){
    DDRD |= _BV(PD6);
    DDRD &= ~_BV(PD2);
    DDRB |= _BV(PB5);
    DDRB |= _BV(PB3);
    DDRC |= _BV(PC1);
    DDRC |= _BV(PC2);

    while (1){
        int idx = 0;
        while(idx < 16){

            if(PIND & _BV(PD2)){
                PORTB &= ~_BV(PB3);
            }
            else{
                PORTB |= _BV(PB3);
            }

            PORTB |= _BV(PB5);
            PORTB &= ~_BV(PB3);
            PORTB &= ~_BV(PB5);
            
            idx += 1;

        }

        PORTC |= _BV(PC2);
        PORTC &= ~_BV(PC1);
    }

}