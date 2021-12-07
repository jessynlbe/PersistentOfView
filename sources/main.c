#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../headers/magnetic.h"
#include "../headers/led.h"

void init_main(){
    sei();
}

void main(void){
    init_main();
    init_magnetic();
    init_leds();
    
    while (1){
    }

}