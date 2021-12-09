#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../headers/magnetic.h"
#include "../headers/usart.h"
#include "../headers/timer.h"
#include "../headers/spi.h"

void main(void){
    init_magnetic();
    init_usart();
    // init_timer2();
    init_spi();

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
        else if(char_received == '3'){
            spi_transmit( (uint8_t) 170, (uint8_t) 170);
        }
        else if(char_received == '4'){
            spi_transmit( (uint8_t) 85, (uint8_t) 85);
        }
        else if(char_received == 'q'){
            break;
        }
        _delay_ms(10);
    }

    spi_transmit( (uint8_t) 0, (uint8_t) 0);
}