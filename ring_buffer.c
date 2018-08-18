#include "ring_buffer.h"


ring_buffer_status_t ring_buffer_push(ring_buffer_t *buffer, uint8_t element)
{
    if (ring_buffer_is_full(buffer))
    {
        ring_buffer_overrun_cb(buffer);

        return RING_BUFFER_STATUS_ERROR;
    }

    buffer->elements[buffer->head_index] = element;

    buffer->head_index = (ring_buffer_size_t) ((buffer->head_index + 1) & RING_BUFFER_MASK);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_t ring_buffer_pop(ring_buffer_t *buffer, uint8_t *element)
{
    if (ring_buffer_is_empty(buffer))
    {
        ring_buffer_underrun_cb(buffer);

        return RING_BUFFER_STATUS_ERROR;
    }

    *element = buffer->elements[buffer->tail_index];

    buffer->tail_index = (ring_buffer_size_t) ((buffer->tail_index + 1) & RING_BUFFER_MASK);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_size_t ring_buffer_push_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size)
{
    ring_buffer_size_t index;

    for (index = 0; index < size; index++)
    {
        if (ring_buffer_push(buffer, elements[index]) != RING_BUFFER_STATUS_OK)
        {
            break;
        }
    }

    return index;
}

ring_buffer_size_t ring_buffer_pop_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size)
{
    ring_buffer_size_t index;

    for (index = 0; index < size; index++)
    {
        if (ring_buffer_pop(buffer, &(elements[index])) != RING_BUFFER_STATUS_OK)
        {
            break;
        }
    }

    return index;
}

__attribute__((weak)) void ring_buffer_overrun_cb(ring_buffer_t *buffer)
{

}

__attribute__((weak)) void ring_buffer_underrun_cb(ring_buffer_t *buffer)
{

}

extern inline bool ring_buffer_is_full(ring_buffer_t *buffer);
extern inline bool ring_buffer_is_empty(ring_buffer_t *buffer);
