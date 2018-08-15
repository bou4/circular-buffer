#include "circular_buffer.h"

void circular_buffer_push_element(circular_buffer_t *buffer, uint8_t element)
{
    if (circular_buffer_is_full(buffer)) {
        circular_buffer_overrun_callback(buffer);

        return;
    }

    buffer->elements[buffer->head_index] = element;

    buffer->head_index = ((buffer->head_index + 1) & CIRCULAR_BUFFER_MASK);
}

void circular_buffer_push_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t element_count)
{
    for (circular_buffer_size_t element_index = 0; element_index < element_count; element_index++) {
        circular_buffer_push_element(buffer, elements[element_index]);
    }
}

uint8_t circular_buffer_pop_element(circular_buffer_t *buffer, uint8_t *element)
{
    if (circular_buffer_is_empty(buffer)) {
        circular_buffer_underrun_callback(buffer);

        return 0;
    }

    *element = buffer->elements[buffer->tail_index];

    buffer->tail_index = ((buffer->tail_index + 1) & CIRCULAR_BUFFER_MASK);

    return 1;
}

uint8_t circular_buffer_pop_elements(circular_buffer_t *buffer, uint8_t *elements, circular_buffer_size_t maximal_element_count)
{
    for (circular_buffer_size_t element_index = 0; element_index < maximal_element_count; element_index++) {
        if (circular_buffer_pop_elements(buffer, &(elements[element_index])) == 0) {
            return element_index;
        }
    }
}

__weak void circular_buffer_overrun_callback(circular_buffer_t *buffer)
{

}

__weak void circular_buffer_underrun_callback(circular_buffer_t *buffer)
{

}
