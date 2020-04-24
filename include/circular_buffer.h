#ifndef ZEOS_CIRCULAR_BUFFER_H
#define ZEOS_CIRCULAR_BUFFER_H

#define MAX_CB_CAPACITY 128

typedef struct {
    char buffer[MAX_CB_CAPACITY];
    int head;
    int tail;
    int capacity;
} char_circular_buffer;

int char_circular_buffer_push(char_circular_buffer * c, char data);

int char_circular_buffer_pop(char_circular_buffer * c, char * data);

int char_circular_buffer_init(char_circular_buffer * c, int capacity);

#endif //ZEOS_CIRCULAR_BUFFER_H
