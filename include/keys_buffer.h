#ifndef __KEYSBUFFER_H__
#define  __KEYSBUFFER_H__

#define CIRCULAR_BUFFER_SIZE 8

typedef struct {
    char buffer[CIRCULAR_BUFFER_SIZE];
    int head;
    int tail;
} circular_buffer;

int circular_buffer_push(circular_buffer * c, char data);

int circular_buffer_pop(circular_buffer * c, char * data);

void init_circular_buffer(circular_buffer * c);

extern circular_buffer * keys_buffer;

#endif