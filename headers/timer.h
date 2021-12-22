#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAX 180
#define SIZETAB 6

volatile int second_compteur;
volatile long seconds;
volatile int ticks;
volatile long minutes;
volatile int rpm;
volatile int column;
volatile long hours;
volatile int mode;

volatile int mult_minutes;
volatile int mod_five_minutes;
volatile int mod_fifteen_minutes;

volatile int *time_tab[5];
volatile long stored_min ;
volatile long stored_hour;


void init_timer2();
void init_timer1();
void init_timer();
void display(long val);
void increment_column();
long getMin();
long getHour();

int* linkTabAndNumber(int val);
void displayMsg(int start , int size , int column , int nbTab , int sizeSep , int nbSections ,volatile int **tab);
void updateTimeTab(int val , int pos ,volatile int **tab);
void initDigitalClock();
void updateTime();