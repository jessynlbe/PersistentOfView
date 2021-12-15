#include "../headers/timer.h"
#include "../headers/spi.h"
#include "../headers/usart.h"


#include <stdio.h>

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

    // Value to compare : 125 * 1625 = 204081 (Number of times he must count to make 1s)
    //Timer must overflow 125x , at each overflow second_counter is incremented.
    //Once it reaches 1625, 1s has elapsed.
    OCR2A = 125;

}

void display(int val){
    char datas[10];
    sprintf(datas , "%d" , val);
    usart_send_string(datas);
    usart_send_byte('\n');
}

void update(){

    // 1200 RPM
    int rps = rpm / 60; // 20 rps
    float or = 1 / (float) rps; // 0.05
    int or_ms = (int) (or * 1000); // 50

    float tpc = or / MAX; // 0,000833333 : 60 col  -- 0,000416667 : 120
    int tpc_us = (int) (tpc*1000000); // 833,333333333 : 60 col -- 416,666666667 : 120

    long f = 13000000/64; // 203125

    float cycle = 1 / (float) f; // 0,000004923
    float cycle_us = (cycle * 1000000); // 4,923

    float overflow = (tpc*1000000) / cycle_us; //169.0 : 60 -- 84 : 120
    if((int) overflow != 0){
        OCR1A = (int) overflow;
    }
}

ISR(TIMER2_COMPA_vect){
    if(second_compteur++ > 1625){

        second_compteur = 0;
        seconds++;

        rpm = ticks * 60;
        ticks = 0;

        update();
    }
}

void init_timer1(){
     // All bits of TCCR = 0
    TCCR1A = 0;
    TCCR1B = 0;

    // Timer2 mode CTC
    TCCR1B |= _BV(WGM12);

    // Clock / 64 --> 4,9 microsecondes
    TCCR1B |= _BV(CS11);
    TCCR1B |= _BV(CS10);

    // Enable interrupt comparaison
    TIMSK1 |= _BV(OCIE1A);

    OCR1A = 255; // Depends du nb de colonne , rendre dynamique son calcul
}

ISR(TIMER1_COMPA_vect){
    if(column == (getMin() + minutes) * mult_minutes){
        spi_transmit( (uint8_t) 239 , (uint8_t) 255 );
    }
    else if(column == ((getHour() + hours) * mod_five_minutes) ){
        spi_transmit( (uint8_t) 160 , (uint8_t) 255 );
    }
    else if(column % mod_fifteen_minutes == 0){
        spi_transmit( (uint8_t) 176 , (uint8_t) 1 );
    }
    else if(column % mod_five_minutes == 0){
        spi_transmit( (uint8_t) 160 , (uint8_t) 1 );
    }
    else{
        spi_transmit( (uint8_t) 192 , (uint8_t) 1 );
    }

    increment_column();
}

void increment_column(){
    if(column == 0){
        column = MAX-1;
    }
    else{
        column--;
    }
}

void init_timer(){
    second_compteur = 0;
    seconds = 0;
    ticks = 0;
    rpm = 0;
    column = 0;
    minutes = 15;
    hours = 6;

    mult_minutes = MAX / 60;
    mod_five_minutes = 5 * mult_minutes;
    mod_fifteen_minutes = 15 * mult_minutes;

    nb_seconds = 0;

    init_timer1();
    init_timer2();

}

int getMin(){
    return (int) seconds / 60;
}

int getHour(){
    return (int) seconds / 3600;
}