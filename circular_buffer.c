#include "circular_buffer.h"


circular_buffer_status_t circular_buffer_push_element(circular_buffer_t *buffer, uint8_t element)
{
    if (circular_buffer_is_full(buffer))
    {
        circular_buffer_overrun_callback(buffer);

        return CIRCULAR_BUFFER_STATUS_ERROR;
    }

    buffer->elements[buffer->head_index] = element;

    buffer->head_index = (circular_buffer_size_t) ((buffer->head_index + 1) & CIRCULAR_BUFFER_MASK);

    return CIRCULAR_BUFFER_STATUS_OK;
}

circular_buffer_status_t circular_buffer_pop_element(circular_buffer_t *buffer, uint8_t *element)
{
    if (circular_buffer_is_empty(buffer))
    {
        circular_buffer_underrun_callback(buffer);

        return CIRCULAR_BUFFER_STATUS_ERROR;
    }

    *element = buffer->elements[buffer->tail_index];

    buffer->tail_index = (circular_buffer_size_t) ((buffer->tail_index + 1) & CIRCULAR_BUFFER_MASK);

    return CIRCULAR_BUFFER_STATUS_OK;
}

circular_buffer_size_t circular_buffer_push_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t element_count)
{
    circular_buffer_size_t element_index;

    for (element_index = 0; element_index < element_count; element_index++)
    {
        if (circular_buffer_push_element(buffer, elements[element_index]) == CIRCULAR_BUFFER_STATUS_ERROR)
        {
            break;
        }
    }

    return element_index;
}

circular_buffer_size_t circular_buffer_pop_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t maximal_element_count)
{
    circular_buffer_size_t element_index;

    for (element_index = 0; element_index < maximal_element_count; element_index++)
    {
        if (circular_buffer_pop_element(buffer, &(elements[element_index])) == CIRCULAR_BUFFER_STATUS_ERROR)
        {
            break;
        }
    }

    return element_index;
}

__attribute__((weak)) void circular_buffer_overrun_callback(circular_buffer_t *buffer)
{

}

__attribute__((weak)) void circular_buffer_underrun_callback(circular_buffer_t *buffer)
{

}

extern inline bool circular_buffer_is_full(circular_buffer_t *buffer);
extern inline bool circular_buffer_is_empty(circular_buffer_t *buffer);
