#include "../headers/magnetic.h"
#include "../headers/spi.h"

ISR(INT0_vect){
    if(!(PIND & _BV(PD2))){ // HIGH and LOW are inverted because of pull-up
            spi_transmit( (uint8_t) 255, (uint8_t) 255);
        }
        else{
            spi_transmit( (uint8_t) 0, (uint8_t) 0);
    }
}

void init_magnetic(){
    DDRD &= ~_BV(PD2); // Configure PD2 as an input
    PORTD |= _BV(PD2); // Enable pull-up on PD2
    EICRA |= _BV(ISC00); // Any logical change on INT0 generates an interrupt request.
    EIMSK |= _BV(INT0); // Enable external interrupt 0
}

