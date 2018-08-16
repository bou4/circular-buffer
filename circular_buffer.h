#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stdint.h>


#define CIRCULAR_BUFFER_SIZE 128
#define CIRCULAR_BUFFER_MASK (CIRCULAR_BUFFER_SIZE - 1)

#if (CIRCULAR_BUFFER_SIZE & CIRCULAR_BUFFER_MASK) != 0
#error "CIRCULAR_BUFFER_SIZE must be a power of two."
#endif

typedef uint8_t circular_buffer_size_t;

typedef struct circular_buffer
{
    uint8_t elements[CIRCULAR_BUFFER_SIZE];

    circular_buffer_size_t head_index;
    circular_buffer_size_t tail_index;
} circular_buffer_t;

inline bool circular_buffer_is_full(circular_buffer_t *buffer)
{
    return ((buffer->head_index - buffer->tail_index) & CIRCULAR_BUFFER_MASK) == CIRCULAR_BUFFER_MASK;
}

inline bool circular_buffer_is_empty(circular_buffer_t *buffer)
{
    return (buffer->head_index == buffer->tail_index);
}

void circular_buffer_push_element(circular_buffer_t *buffer, uint8_t element);
void circular_buffer_push_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t element_count);

uint8_t circular_buffer_pop_element(circular_buffer_t *buffer, uint8_t *element);
uint8_t circular_buffer_pop_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t maximal_element_count);

void circular_buffer_overrun_callback(circular_buffer_t *buffer);
void circular_buffer_underrun_callback(circular_buffer_t *buffer);

#endif /* CIRCULAR_BUFFER_H */
