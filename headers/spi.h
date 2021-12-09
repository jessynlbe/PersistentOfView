#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int led_on;

void init_spi();
void spi_transmit(uint8_t first , uint8_t second);
void toggle_leds();