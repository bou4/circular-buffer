/*!
 * @file ring_buffer.c
 * @author Caro Meysmans
 *
 * MIT License
 *
 * Copyright (c) 2018 Caro Meysmans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ring_buffer.h"

extern inline bool ring_buffer_is_full(ring_buffer_t *buffer);

extern inline bool ring_buffer_is_empty(ring_buffer_t *buffer);

ring_buffer_status_t ring_buffer_push(ring_buffer_t *buffer, uint8_t element)
{
    if (ring_buffer_is_full(buffer))
    {
        return RING_BUFFER_STATUS_ERROR;
    }

    buffer->elements[buffer->head_index] = element;

    buffer->head_index = (ring_buffer_size_t) ((buffer->head_index + 1U) & RING_BUFFER_MASK);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_t ring_buffer_pop(ring_buffer_t *buffer, uint8_t *element)
{
    if (ring_buffer_is_empty(buffer))
    {
        return RING_BUFFER_STATUS_ERROR;
    }

    *element = buffer->elements[buffer->tail_index];

    buffer->tail_index = (ring_buffer_size_t) ((buffer->tail_index + 1U) & RING_BUFFER_MASK);

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
