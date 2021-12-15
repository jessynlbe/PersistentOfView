#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../headers/magnetic.h"
#include "../headers/usart.h"
#include "../headers/timer.h"
#include "../headers/spi.h"
#include <string.h>
	
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void main(void){
    init_magnetic();
    init_usart();
    init_timer();
    init_spi();
    sei();

    usart_send_string("Start clock\n");
    usart_send_string("r : Read rx_buffer\n");
    usart_send_string("t : Read rx_buffer\n");
    usart_send_string("h : Write the time in the form hh:mm validate then enter h\n");
    while (1){
        char char_received = usart_read();
        if(char_received == 'r')
        {
            usart_read_buffer(&rx_rbuffer);
        }
        else if(char_received == 't'){
            usart_read_buffer(&tx_rbuffer);
        }
        else if(char_received == 'h'){
            int start = rx_rbuffer.read_pos -1;

            char h1 = rx_rbuffer.buffer[start-7];
            char h2 = rx_rbuffer.buffer[start-6];

            
            char hours_tab[3] = {h1,h2,'\0'};
            hours = atoi(hours_tab) % 12;

            h1 = rx_rbuffer.buffer[start-4];
            h2 = rx_rbuffer.buffer[start-3];

            char min_tab[3] = {h1, h2 , '\0'};
            minutes = atoi(min_tab);

            seconds = 0; // Reset the seconds counter to zero
            TCNT1 = 0; // Reset the rpm counter (timer 1)
            TCNT2 = 0; // Reset the seconds counter (timer 2)

        }
    }

}