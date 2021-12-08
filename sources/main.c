#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../headers/magnetic.h"
#include "../headers/led.h"
#include "../headers/usart.h"
#include "../headers/timer.h"

void main(void){
    init_magnetic();
    init_leds();
    init_usart();
    init_timer2();

    usart_send_string("Init finished\n");
    _delay_ms(10);
    usart_send_string("1 : Read content of rx_buffer\n");
    _delay_ms(10);
    usart_send_string("2 : Read content of tx_buffer\n");
    while (1){
        uint8_t char_received = usart_read();
        if(char_received == '1')
        {
            usart_read_buffer(&rx_rbuffer);
        }
        else if(char_received == '2'){
            usart_read_buffer(&tx_rbuffer);
        }
        _delay_ms(10);
    }

}