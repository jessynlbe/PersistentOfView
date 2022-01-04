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

volatile uint8_t *time_tab[5];
volatile long stored_min ;
volatile long stored_hour;
volatile int nb_seconds;

volatile long stored_min2 ;
volatile long stored_hour2;



void init_timer2();
void init_timer1();
void init_timer();
void display(long val);
void increment_column();
long getMin();
long getHour();

uint8_t* linkTabAndNumber(int val);
void displayMsg(int start , int size , int column , int nbTab , int sizeSep , int nbSections ,volatile uint8_t **tab);
void updateTimeTab(int val , int pos ,volatile uint8_t **tab);
void initDigitalClock();
void updateTime();

void init_array();
void init_final();
void copy_array(uint8_t *src, uint8_t *dst , int size);
void update_bits_hour();

uint16_t* chooseTab(int val);
void updateTimeTab2(int hour_val , int min_val);
void updateTime2();
void init_temp_array();

void init_timer0();
void stop_timer();
void calculate();