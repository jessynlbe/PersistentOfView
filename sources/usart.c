#include "../headers/usart.h"

#define SIZE 128

#define F_CPU 13000000
#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

///////////////////////// Ring Buffer ////////////////////////

int incrementPos(int pos){
    if(pos >= SIZE){
        return 0;
    }
    return ++pos;
}

void buffer_write(ringBuffer *rb, uint8_t c)
{
    rb->buffer[rb->write_pos] = c;
    rb->write_pos = incrementPos(rb->write_pos);
}

void buffer_string(ringBuffer *rb, uint8_t *str)
{
    for(int i = 0 ; str[i] != '\0' ; i++){
        buffer_write(rb,str[i]);
    }
}

uint8_t buffer_read(ringBuffer *rb)
{
    uint8_t data = rb->buffer[rb->read_pos];
    rb->read_pos = incrementPos(rb->read_pos);
    return data;
}

////////////////////////////////// USART ///////////////////////////

void USART_Init(unsigned int ubrr){
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0A &= ~(1<<U2X0);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    sei();
}



bool allDataSend(ringBuffer *rb){
    if(rb->write_pos == rb->read_pos){
        return true;
    }
    return false;
}

bool dataAvailable(ringBuffer *rb){
    if(rb->write_pos != rb->read_pos){
        return true;
    }
    return false;
}

ISR(USART_UDRE_vect)
{
    UDR0 = buffer_read(&tx_rbuffer);
    if (allDataSend(&tx_rbuffer))
    {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

ISR(USART_RX_vect)
{
    buffer_write(&rx_rbuffer , UDR0);
}

void usart_send_byte(uint8_t byte){
    buffer_write(&tx_rbuffer, byte);
    UCSR0B |= _BV(UDRIE0);

}

void usart_send_string(uint8_t *str){
    buffer_string(&tx_rbuffer , str);
    UCSR0B |= _BV(UDRIE0);
}

uint8_t usart_read(){
    UCSR0B |= _BV(RXCIE0);
    if(dataAvailable(&rx_rbuffer)){
        return buffer_read(&rx_rbuffer);
    }
}

void usart_read_buffer(ringBuffer *rb){
    usart_send_string("\nStart reading buffer :\n");
    _delay_ms(10);
    int max = rb->write_pos ;
    int idx = 0;
    int val = 1;
    while(idx < max){
        uint8_t value = rb->buffer[idx];
        usart_send_byte(value);
        idx+=1;
    }

    usart_send_byte('\n');
}

void init_usart(){
    USART_Init(MYUBRR);
    tx_rbuffer.write_pos = 0;
    tx_rbuffer.read_pos = 0;

    rx_rbuffer.write_pos = 0;
    rx_rbuffer.read_pos = 0;
}