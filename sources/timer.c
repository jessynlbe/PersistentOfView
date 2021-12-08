#include "../headers/timer.h"
#include "../headers/led.h"

void init_timer2(){
    // All bits of TCCR = 0
    TCCR2A = 0;
    TCCR2B = 0;

    // Timer2 mode CTC
    TCCR2A |= _BV(WGM21);

    // Clock / 64 --> 4,9 microsecondes
    TCCR2B |= _BV(CS22);

    // Enable interrupt comparaison
    TIMSK2 |= _BV(OCIE2A);

    // Value to compare : 177 * 1153 = 204081 (Number of times he must count to make 1s)
    //Timer must overflow 177x , at each overflow second_counter is incremented.
    //Once it reaches 1153, 1s has elapsed.
    OCR2A = 177;

    second_compteur = 0;
}

ISR(TIMER2_COMPA_vect){
    if(second_compteur++ > 1153){
        second_compteur = 0;
        toggle_leds();
    }
}