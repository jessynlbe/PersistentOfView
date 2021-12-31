#include "../headers/magnetic.h"
#include "../headers/spi.h"
#include "../headers/usart.h"
#include "../headers/timer.h"

ISR(INT0_vect){
    ticks ++; 

    // Each time the sensor is passed, the display of the columns is reset to 0 and the display counter of each column can be reset to zero to synchronize
    column = 0;
    TCNT1 = 0;
}

void init_magnetic(){
    DDRD &= ~_BV(PD2); // Configure PD2 as an input
    PORTD |= _BV(PD2); // Enable pull-up on PD2
    EICRA &= ~_BV(ISC00); // Falling edge
    EICRA |= _BV(ISC01); // Falling edge
    EIMSK |= _BV(INT0); // Enable external interrupt 0
}

