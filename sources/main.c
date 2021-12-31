
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "../headers/magnetic.h"
#include "../headers/usart.h"
#include "../headers/timer.h"
#include "../headers/spi.h"
#include "../headers/bits.h"
#include "../headers/numbers.h"


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
    usart_send_string("m : Write the mode of the form x validate and then enter m\n");
    _delay_ms(10);
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
            char h3 = rx_rbuffer.buffer[start-4];
            char h4 = rx_rbuffer.buffer[start-3];
            
            char hours_tab[3] = {h1,h2,'\0'};
            char min_tab[3] = {h3, h4 , '\0'};

            if(mode == 0){
                hours = atoi(hours_tab);
                minutes = atoi(min_tab);
                seconds = (3600 * hours) + (60*minutes);// Reset the seconds counter to zero
            }
            else if(mode == 1){
                hours = atoi(hours_tab);
                minutes = atoi(min_tab);
                seconds = (3600 * hours) + (60*minutes);
                initDigitalClock();
            }
            else if(mode == 2){
                hours = atoi(hours_tab);
                minutes = atoi(min_tab);
                seconds = (3600 * hours) + (60*minutes);
                updateTimeTab2(hours , minutes);
            }

            TCNT1 = 0; // Reset the rpm counter (timer 1)
            TCNT2 = 0; // Reset the seconds counter (timer 2)
        }
        else if(char_received == 'm'){
            char m[2]= {rx_rbuffer.buffer[rx_rbuffer.read_pos - 4] , '\0'};
            mode = atoi(m);

        }
    }

}