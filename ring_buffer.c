#include "ring_buffer.h"


ring_buffer_status_t ring_buffer_push_element(ring_buffer_t *buffer, uint8_t element)
{
    if (ring_buffer_is_full(buffer))
    {
        ring_buffer_overrun_callback(buffer);

        return RING_BUFFER_STATUS_ERROR;
    }

    buffer->elements[buffer->head_index] = element;

    buffer->head_index = (ring_buffer_size_t) ((buffer->head_index + 1) & RING_BUFFER_MASK);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_t ring_buffer_pop_element(ring_buffer_t *buffer, uint8_t *element)
{
    if (ring_buffer_is_empty(buffer))
    {
        ring_buffer_underrun_callback(buffer);

        return RING_BUFFER_STATUS_ERROR;
    }

    *element = buffer->elements[buffer->tail_index];

    buffer->tail_index = (ring_buffer_size_t) ((buffer->tail_index + 1) & RING_BUFFER_MASK);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_size_t ring_buffer_push_elements(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t element_count)
{
    ring_buffer_size_t element_index;

    for (element_index = 0; element_index < element_count; element_index++)
    {
        if (ring_buffer_push_element(buffer, elements[element_index]) != RING_BUFFER_STATUS_OK)
        {
            break;
        }
    }

    return element_index;
}

ring_buffer_size_t ring_buffer_pop_elements(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t maximal_element_count)
{
    ring_buffer_size_t element_index;

    for (element_index = 0; element_index < maximal_element_count; element_index++)
    {
        if (ring_buffer_pop_element(buffer, &(elements[element_index])) != RING_BUFFER_STATUS_OK)
        {
            break;
        }
    }

    return element_index;
}

__attribute__((weak)) void ring_buffer_overrun_callback(ring_buffer_t *buffer)
{

}

__attribute__((weak)) void ring_buffer_underrun_callback(ring_buffer_t *buffer)
{

}

extern inline bool ring_buffer_is_full(ring_buffer_t *buffer);
extern inline bool ring_buffer_is_empty(ring_buffer_t *buffer);
