#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 125

typedef struct ringBuffer ringBuffer;

struct ringBuffer {
    uint8_t read_pos;
    uint8_t write_pos;
    uint8_t buffer[SIZE];
};

ringBuffer tx_rbuffer;
ringBuffer rx_rbuffer;

uint8_t incrementPos(uint8_t pos);
void buffer_write(ringBuffer *rb, uint8_t c);
void buffer_string(ringBuffer *rb, uint8_t *str);
uint8_t buffer_read(ringBuffer *rb);
void USART_Init();
bool allDataSend(ringBuffer *rb);
bool dataAvailable(ringBuffer *rb);
void usart_send_byte(uint8_t byte);
void usart_send_string(uint8_t *str);
uint8_t usart_read();
void init_usart();
void usart_read_buffer(ringBuffer *rb);
