#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void init_leds();
void turn_on_leds();
void turn_off_leds();