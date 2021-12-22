#include "../headers/timer.h"
#include "../headers/spi.h"
#include "../headers/usart.h"

#include <math.h>
#include <stdio.h>

//////////////////// Tab of numbers ///////////////////////////:
int zero[12] = {15,240,16,8,16,8,16,8,16,8,15,240};
int one[12] = {4,8,8,8,31,248,31,248,0,8,0,8};
int two[12] = {4,8,8,24,16,40,16,72,16,136,15,8};
int three[12] = {16,16,16,8,16,8,19,8,20,136,24,112};
int four[12] = {1,128,2,128,4,128,8,136,31,248,0,136};
int five[12] = {31,8,17,8,17,8,17,8,17,8,16,240};
int six[12] = {1,248,2,136,4,136,8,136,16,136,0,248};
int seven[12] = {16,0,16,0,16,0,16,0,16,0,31,248};
int eight[12] = {15,240,17,8,17,8,17,8,17,8,15,240};
int nine[12] = {30,8,18,8,18,8,18,8,18,8,31,248};

int point[12] = {0,0,0,0,6,96,6,96,0,0,0,0};

/////////////////// All initialisation /////////////////////////


void init_timer(){
    second_compteur = 0;
    ticks = 0;
    rpm = 0;
    column = 0;

    minutes = 15;
    hours = 6;
    stored_min = minutes;
    stored_hour = hours;

    seconds = (3600 * (long) hours) + (60* (long) minutes);

    mult_minutes = MAX / 60; // Value by which the minutes must be multiplied because of the increased number of columns
    mod_five_minutes = 5 * mult_minutes; // Modulo to obtain the 5min intervals
    mod_fifteen_minutes = 15 * mult_minutes; // Modulo to obtain the 15min intervals

    init_timer1();
    init_timer2();

    mode = 0;
    time_tab[2] = point;
    updateTimeTab(hours , 0 , time_tab);
    updateTimeTab(minutes , 3 , time_tab);
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

void displayMsg(int start , int size , int column , int nbTab , int sizeSep , int nbSections ,volatile int **tab){
    int sizeBeforeZero = MAX - start; // Combien de colonnes avant de devoir repasser à zero
    int end =  size - sizeBeforeZero -1; // Nombre de colonnes qui depasse zero (end est alors la valeur de la derniere colonne du message)
    int sizeSections = SIZETAB + (sizeSep); // Taille d'une section contenant un tab et un sep

    int diff = 0;
    if(column <= end){ // Si colonne est inferieur a end alors on doit convertir pour pouvoir faire la difference
        int value = MAX + column; // Exemple si colonne = 1 et MAX = 180 , vlaue = 181 donc on peut savoir la diff 181 - start;
        diff = value - start;
    }
    else if(column >= start && column < MAX){ // Si colonne est entre le start et le MAX (savoir si on doit convertir la colonne)
        diff = column - start;
    }
    else{
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); // Si on est dans aucun de ses cas cest qu'on est en dehors de la zzero du message donc 0 partout
        return;
    }

    if(diff > size){ // Si on est ici cest qu'on est en dehors de la zzero du message
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); 
        return;
    }

    int idxSection = diff / sizeSections; // L'index de la section ou se trouve la colonne
    int offsetSection = diff % sizeSections; // Le décalage dans la section ou se trouve la colonne

    if(offsetSection >= SIZETAB){
        spi_transmit( (uint8_t) 128 , (uint8_t) 1 ); 
        return;
    }

    int idxInTab = offsetSection * 2;
    spi_transmit( (uint8_t) tab[idxSection][idxInTab] +128 , (uint8_t) tab[idxSection][idxInTab+1] +1);

}

int* linkTabAndNumber(int val){
    
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

void updateTimeTab(int val , int pos ,volatile int **tab){
    int first = val / 10;
    int snd = val%10;

    tab[pos] = linkTabAndNumber(first);
    tab[pos+1] = linkTabAndNumber(snd);
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

long getMin(){
    long value =  seconds - (3600* getHour());
    long ret = (long) value / 60;
    return ret;
}

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
    if((int) overflow != 0){
        OCR1A = (int) overflow;
    }
}


///////////////////////////// ISR ///////////////////////////////

ISR(TIMER2_COMPA_vect){
    if(second_compteur++ > 1625){

        second_compteur = 0;

        if(seconds >= 86399){
            seconds = 0;
            minutes = 0;
            hours = 0;
        }
        else{
            seconds++;
        }

        rpm = ticks * 60;
        ticks = 0;

        update();
    }
}


ISR(TIMER1_COMPA_vect){
    if(mode == 0){
        int min = getMin();
        int hour = getHour();

        if(hour > 12){
            hour = hour % 12;
        }

        if(column == min * mult_minutes){
            spi_transmit( (uint8_t) 239 , (uint8_t) 255 );
        }
        else if(column == hour * mod_five_minutes){
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
    }
    else if(mode == 1){
        int mult = MAX/60;
        int nbTab = 5;
        int sizeSep = (mult*2);
        int nbSections = nbTab-1;
        int size = (nbTab * SIZETAB) + (nbSections*sizeSep);

        updateTime();
        displayMsg(MAX-30 , size , column , nbTab , sizeSep , nbSections , time_tab);
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