#include <circular_buffer.h>

int char_circular_buffer_push(char_circular_buffer * c, char data)
{
    int next;

    next = c->head + 1;
    if(next >= c->capacity) next = 0;

    if(next == c->tail) return -1; //Circular buffer full, error

    c->buffer[c->head] = data;
    c->head = next;

    return 0; //Success
}

int char_circular_buffer_pop(char_circular_buffer * c, char * data)
{
    int next;

    if(c->head == c->tail) return -1; //Circular buffer empty, error

    next = c->tail + 1;
    if(next >= c->capacity) next = 0;

    *data = c->buffer[c->tail];
    c->tail = next;

    return 0; //Success
}

int char_circular_buffer_init(char_circular_buffer * c, int capacity)
{
    if (capacity > MAX_CB_CAPACITY)
    {
        return -1;
    }
    c->head = 0;
    c->tail = 0;
    c->capacity = capacity;
    return 0;
}
