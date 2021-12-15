#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAX 120

volatile int second_compteur;
volatile int seconds;
volatile int ticks;
volatile int minutes;
volatile int rpm;
volatile int column;
volatile int hours;

volatile int mult_minutes;
volatile int mod_five_minutes;
volatile int mod_fifteen_minutes;

volatile int nb_seconds;


void init_timer2();
void init_timer1();
void init_timer();
void display(int val);
void increment_column();
int getMin();
int getHour();