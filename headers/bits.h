#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <avr/pgmspace.h>

#define SLOTS 180

int getBits(unsigned long pos , unsigned long number);
void fillTab(int startBits , int offH , int offW ,const uint16_t tab[11]);
int convertPolarToCartesian(int rad , int angle , int size , int value);
int convertCartToArr(int x , int y , int size , int value);
int getBitsTab(uint32_t *tab , int x , int y);
void getLeds(int nbLeds , int nbSlots , uint32_t *tab , int size  ,uint8_t *arr);

uint32_t final_b[32];

uint8_t arr_b[SLOTS*2];
uint8_t temp_arr_b[SLOTS*2];