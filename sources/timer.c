#include "../headers/timer.h"
#include "../headers/spi.h"
#include "../headers/usart.h"
#include "../headers/bits.h"
#include "../headers/numbers.h"

#include <math.h>
#include <stdio.h>

/////////////////// All initialisation /////////////////////////

void init_array(){
    for(int i = 0 ; i < SLOTS*2 ; i++){
        arr_b[i] = 0;
    }
}

void init_temp_array(){
    for(int i = 0 ; i < SLOTS*2 ; i++){
        temp_arr_b[i] = 0;
    }
}

// Recalculates the value of each led for each column
// frst : Dizaine of the hour
// snd : Decimal of hours
// thrd : Dizaine of min
// frth : Decimal of min
void update_bits_hour(uint16_t *frst , uint16_t * snd , uint16_t * thrd , uint16_t *frth){
    fillTab(11 , 4 , 16 , frst); // haut gauche
    fillTab(11 , 18, 5 , frth); // bas droit
    
    fillTab(11 , 18 , 16 , thrd); // bas gauche
    fillTab(11 , 4 , 5 , snd); // haut droit
    getLeds(16 , SLOTS , final_b , 32 , temp_arr_b);
    copy_array(temp_arr_b , arr_b , SLOTS*2);
}

void init_final(){
    for(int i = 0 ; i < 32 ; i++){
        final_b[i] = 0;
    }
}

void copy_array(uint8_t *src, uint8_t *dst , int size){
    for(int i = 0 ; i < SLOTS*2 ; i++){
        dst[i] = src[i];
    }
}

void init_timer(){
    second_compteur = 0;
    ticks = 0;
    rpm = 0;
    column = 0;

    minutes = 26;
    hours = 8;
    stored_min = minutes;
    stored_hour = hours;

    stored_min2 = minutes;
    stored_hour2 = hours;

    seconds = (3600 * (long) hours) + (60* (long) minutes);

    mult_minutes = MAX / 60; // Value by which the minutes must be multiplied because of the increased number of columns
    mod_five_minutes = 5 * mult_minutes; // Modulo to obtain the 5min intervals
    mod_fifteen_minutes = 15 * mult_minutes; // Modulo to obtain the 15min intervals

    nb_seconds = 0;

    init_timer1();
    init_timer2();

    mode = 0;

    time_tab[2] = point;
    updateTimeTab(hours , 0 , time_tab);
    updateTimeTab(minutes , 3 , time_tab);

    updateTimeTab2(hours , minutes);

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

void initDigitalClock(){
    updateTimeTab(hours, 0 , time_tab);
    updateTimeTab(minutes , 3 , time_tab);
}


//////////////////// Digital Clock /////////////////

// Allows you to set the hours and minutes on the clock
// start : start column for time display
// size : Size of time display
// column : Column to be displayed
// nbTab : Number of tabs int the time display ( 5 --> 23:56)
// sizeSep : Size of the separator between each number
// nbSections : Number of sections ( one section = one separator and one number)
// tab : Table in which to find the values of the leds
void displayMsg(int start , int size , int column , int nbTab , int sizeSep , int nbSections ,volatile uint8_t **tab){
    int sizeBeforeZero = MAX - start; // Combien de colonnes avant de devoir repasser à zero
    int end =  size - sizeBeforeZero -1; // Nombre de colonnes qui depassent zero (end est alors la valeur de la derniere colonne du message)
    int sizeSections = SIZETAB + (sizeSep); // Taille d'une section contenant un tab et un sep

    int diff = 0;
    if(column <= end){ // Si column est inferieure a end alors on doit convertir pour pouvoir faire la difference
        int value = MAX + column; // Exemple : si column = 1 et MAX = 180, value = 181 donc on peut déterminer la différence 181 - start;
        diff = value - start;
    }
    else if(column >= start && column < MAX){ // Si column est entre le start et le MAX (pour savoir si on doit convertir la colonne)
        diff = column - start;
    }
    else{
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); // Si on est dans aucun de ses cas c'est qu'on est en dehors de la zeero du message donc 0 partout
        return;
    }

    if(diff > size){ // Si on est ici cest qu'on est en dehors de la zzero du message
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); 
        return;
    }

    int idxSection = diff / sizeSections; // L'index de la section où se trouve la colonne
    int offsetSection = diff % sizeSections; // Le décalage dans la section où se trouve la colonne

    if(offsetSection >= SIZETAB){
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); 
        return;
    }

    int idxInTab = offsetSection * 2;
    spi_transmit( (uint8_t) tab[idxSection][idxInTab] +128 , (uint8_t) tab[idxSection][idxInTab+1] +1);

}

uint8_t* linkTabAndNumber(int val){
    
    switch(val){
        case 0:
            return zero;
        case 1:
            return one;
        case 2:
            return two;
        case 3:
            return three;
        case 4:
            return four;
        case 5:
            return five;
        case 6:
            return six;
        case 7:
            return seven;
        case 8:
            return eight;
        case 9:
            return nine;
    }
    
}

// Update the clock every minute / hour. To recalculate only when necessary
void updateTime(){
    if( (getMin()) != stored_min){
        updateTimeTab(getMin() , 3 , time_tab);
        stored_min = getMin();
    }

    if( (getHour()) != stored_hour){
        updateTimeTab(getHour() , 0 , time_tab);
        stored_hour = getHour();
    }
}

// Allows to update the array which contains the values of the leds to light
// val : Value to be displayed
// pos : Position in the table of numbers
// tab : Table of tables
void updateTimeTab(int val , int pos ,volatile uint8_t **tab){
    int first = val / 10;
    int snd = val%10;

    tab[pos] = linkTabAndNumber(first);
    tab[pos+1] = linkTabAndNumber(snd);
}


/////////////////// Digital Clock without distortion //////////////////

// Update the clock every minute / hour. To recalculate only when necessary
void updateTime2(){
    if( (getMin()) != stored_min2){
        updateTimeTab2(getHour() , getMin());
        stored_min2 = getMin();
    }

    if( (getHour()) != stored_hour2){
        updateTimeTab2(getHour(), getMin());
        stored_hour2 = getHour();
    }
}

uint16_t* chooseTab(int val){
    switch(val){
        case 0:
            return zero_b;
        case 1:
            return one_b;
        case 2:
            return two_b;
        case 3:
            return three_b;
        case 4:
            return four_b;
        case 5:
            return five_b;
        case 6:
            return six_b;
        case 7:
            return seven_b;
        case 8:
            return eight_b;
        case 9:
            return nine_b;
    }
}

// Allows to update the array which contains the values of the leds to light
void updateTimeTab2(int hour_val , int min_val){
    // Recovery of each digit composing the hours and minutes
    int firstH = hour_val / 10;
    int sndH = hour_val%10;

    int firstM = min_val/10;
    int sndM = min_val%10;

    // Cleaning of the table containing the "drawing" of the numbers
    init_final();
    init_temp_array();

    update_bits_hour(
        chooseTab(firstH),
        chooseTab(sndH),
        chooseTab(firstM),
        chooseTab(sndM)
     );
}

////////////////////// Additional functions //////////////////////////

void increment_column(){
    if(column == 0){
        column = MAX-1;
    }
    else{
        column--;
    }
}

// Conversion of seconds to minutes
long getMin(){
    long value =  seconds - (3600* getHour());
    long ret = (long) value / 60;
    return ret;
}

// Conversion of seconds to hours
long getHour(){
    long value = (long) seconds / 3600;
    return value;
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
    OCR1A = (int) overflow+1;
}


///////////////////////////// ISR ///////////////////////////////

ISR(TIMER2_COMPA_vect){
    if(second_compteur++ > 1625){
        
        second_compteur = 0;

        // Resets all variables to zero when 23h59m59s is reached.
        if(seconds >= 86399){
            seconds = 0;
            minutes = 0;
            hours = 0;
        }
        else{
            seconds++;
        }

       // RPM update every second.
       // Calculation of the number of ticks ( 1 at each passage in front of the sensor) in 1 min ( x 60 ) 
        rpm = ticks * 60;
        ticks = 0;

        update();
    }
}


ISR(TIMER1_COMPA_vect){
    if(mode == 0){
        int min = getMin();
        int hour = getHour();

        // Clock 1, if we exceed 12h then we convert the hours on 12h rather than 24
        if(hour >= 12){
            hour = hour % 12;
        }

        if(column == (min*mult_minutes) && column % mod_five_minutes == 0){
            spi_transmit( (uint8_t) 231 , (uint8_t) 255 );
        }
        else if(column == min * mult_minutes){ // Needle min
            spi_transmit( (uint8_t) 199 , (uint8_t) 255 );
        }
        else if(column == hour * mod_five_minutes){ // Needle hour
            spi_transmit( (uint8_t) 160 , (uint8_t) 255 );
        }
        else if(column % mod_fifteen_minutes == 0){ // Drawing of 15min intervals
            spi_transmit( (uint8_t) 176 , (uint8_t) 1 );
        }
        else if(column % mod_five_minutes == 0){ // Drawing of 5min intervals
            spi_transmit( (uint8_t) 160 , (uint8_t) 1 );
        }
        else{
            spi_transmit( (uint8_t) 192 , (uint8_t) 1 );
        }
    }
    else if(mode == 1){
        int mult = MAX/60;
        // Number of tab to display : Ex - 12:43 so 5
        int nbTab = 5;
        // Size between each tab
        int sizeSep = (mult*2);
        // Number of separators
        int nbSections = nbTab-1;
        // Global size of tab containing hours , min and double dot
        int size = (nbTab * SIZETAB) + (nbSections*sizeSep);

        updateTime();
        displayMsg(MAX-30 , size , column , nbTab , sizeSep , nbSections , time_tab);
    }
    else if(mode == 2){
        int start = column * 2;
        spi_transmit( arr_b[start+1] ,  arr_b[start] );
        updateTime2();
        
    }
    increment_column();
}

////////////////////// Debug ///////////////////////////

void display(long val){
    char datas[10];
    sprintf(datas , "%ld" , val);
    usart_send_string(datas);
    usart_send_byte('\n');
}


/////////////////// Calculate number cycles //////////////////:

long val = 0;

void init_timer0(){
    sei();
     // All bits of TCCR = 0
    TCCR0A = 0;
    TCCR0B = 0;

    TCCR0B |= _BV(CS00);
    TCCR0B |= _BV(CS01);

    // Enable interrupt comparaison
    TIMSK0 |= _BV(TOIE0);

}

ISR(TIMER0_OVF_vect){
    val++;
}

void stop_timer(){
    cli();
    long v = val * (long) 255 + (long) TCNT0;
    display(v);
    TCCR0B &= ~_BV(CS00);
    sei();
}

void calculate(){
    init_timer0();
    // fonction to calculate
    stop_timer();
}