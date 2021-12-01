#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void main(void){
    DDRD |= _BV(PD6);
    while(1){
        PORTD |= _BV(PD6);
        _delay_ms(1000);
        PORTD &= ~_BV(PD6);
        _delay_ms(1000);
    }
}