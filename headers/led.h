#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int led_on;

void init_leds();
void turn_on_leds();
void turn_off_leds();
void toggle_leds();