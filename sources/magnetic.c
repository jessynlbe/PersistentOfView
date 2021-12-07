#include "../headers/magnetic.h"
#include "../headers/led.h"

ISR(INT0_vect){
    if(!(PIND & _BV(PD2))){ // HIGH and LOW are inverted because of pull-up
            turn_on_leds();
        }
        else{
            turn_off_leds();
    }
}

void init_magnetic(){
    DDRD &= ~_BV(PD2); // Configure PD2 as an input
    PORTD |= _BV(PD2); // Enable pull-up on PD2
    EICRA |= _BV(ISC00); // Any logical change on INT0 generates an interrupt request.
    EIMSK |= _BV(INT0); // Enable external interrupt 0
}

