#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

#define RING_BUFFER_SIZE 128
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)

#if (RING_BUFFER_SIZE & RING_BUFFER_MASK) != 0
#error "RING_BUFFER_SIZE must be a power of two."
#endif

typedef uint8_t ring_buffer_size_t;

typedef struct ring_buffer
{
    uint8_t elements[RING_BUFFER_SIZE];

    ring_buffer_size_t head_index;
    ring_buffer_size_t tail_index;
} ring_buffer_t;

typedef enum ring_buffer_status
{
    RING_BUFFER_STATUS_OK,
    RING_BUFFER_STATUS_ERROR
} ring_buffer_status_t;

inline bool ring_buffer_is_full(ring_buffer_t *buffer)
{
    return ((buffer->head_index - buffer->tail_index) & RING_BUFFER_MASK) == RING_BUFFER_MASK;
}

inline bool ring_buffer_is_empty(ring_buffer_t *buffer)
{
    return (buffer->head_index == buffer->tail_index);
}

ring_buffer_status_t ring_buffer_push(ring_buffer_t *buffer, uint8_t element);
ring_buffer_status_t ring_buffer_pop(ring_buffer_t *buffer, uint8_t *element);

ring_buffer_size_t ring_buffer_push_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size);
ring_buffer_size_t ring_buffer_pop_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size);

void ring_buffer_overrun_cb(ring_buffer_t *buffer);
void ring_buffer_underrun_cb(ring_buffer_t *buffer);

#endif /* RING_BUFFER_H */
