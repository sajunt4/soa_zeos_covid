#include <keys_buffer.h>

int circular_buffer_push(circular_buffer * c, char data)
{
    int next;

    next = c->head + 1;
    if(next >= CIRCULAR_BUFFER_SIZE) next = 0;

    if(next == c->tail) return -1; //Circular buffer full, error

    c->buffer[c->head] = data;
    c->head = next;

    return 1; //Success
}

int circular_buffer_pop(circular_buffer * c, char * data)
{
    int next;

    if(c->head == c->tail) return -1; //Circular buffer empty, error

    next = c->tail + 1;
    if(next >= CIRCULAR_BUFFER_SIZE) next = 0;

    *data = c->buffer[c->tail];
    c->tail = next;

    return 1; //Success
}

void init_circular_buffer(circular_buffer * c)
{
    c->head = 0;
    c->tail = 0;
}