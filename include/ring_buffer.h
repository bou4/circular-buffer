/*!
 * @file ring_buffer.h
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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/*!
 * The ring buffer can only contain <tt>RING_BUFFER_SIZE - 1</tt> elements,
 * to be able to distinguish between a full and empty ring buffer.
 *
 * @c RING_BUFFER_SIZE must be a power of two, because we depend on the property,
 * <tt>a % 2^n = a & (2^n - 1)</tt>.
 *
 * @c RING_BUFFER_SIZE must fit in @c ring_buffer_size_t.
 */
#define RING_BUFFER_SIZE 128U
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1U)

#if (RING_BUFFER_SIZE & RING_BUFFER_MASK) != 0
#error "RING_BUFFER_SIZE must be a power of two."
#endif

/*!
 * \c ring_buffer_size_t is used to represent the size of the ring buffer.
 * As an implication, @c ring_buffer_size_t can hold any ring buffer index.
 */
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

/*!
 * Check if the ring buffer is full.
 * @param buffer The buffer to check.
 * @return @c true if the buffer is full.
 */
inline bool ring_buffer_is_full(ring_buffer_t *buffer)
{
    return (((ring_buffer_size_t) (buffer->head_index - buffer->tail_index)) & RING_BUFFER_MASK) == RING_BUFFER_MASK;
}

/*!
 * Check if the ring buffer is empty.
 * @param buffer The buffer to check.
 * @return @c true if the buffer is empty.
 */
inline bool ring_buffer_is_empty(ring_buffer_t *buffer)
{
    return (buffer->head_index == buffer->tail_index);
}

/*!
 * Add one element to the ring buffer.
 * @param buffer The buffer to write to.
 * @param element The element to add.
 * @return @c RING_BUFFER_STATUS_OK if an element was added.
 */
ring_buffer_status_t ring_buffer_push(ring_buffer_t *buffer, uint8_t element);

/*!
 * Remove one element from the ring buffer.
 * @param buffer The buffer to read from.
 * @param element The removed element.
 * @return @c RING_BUFFER_STATUS_OK if an element was removed.
 */
ring_buffer_status_t ring_buffer_pop(ring_buffer_t *buffer, uint8_t *element);

/*!
 * Add one or more elements to the ring buffer.
 * No more elements are added than possible.
 * @param buffer The buffer to write to.
 * @param elements The elements to add.
 * @param size The number of elements to add.
 * @return The number of elements added.
 */
ring_buffer_size_t ring_buffer_push_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size);

/*!
 * Remove and store one or more elements from the ring buffer.
 * No more elements are removed than possible.
 * @param buffer The buffer to read from.
 * @param elements The removed elements.
 * @param size The number of elements to remove.
 * @return The number of elements removed.
 */
ring_buffer_size_t ring_buffer_pop_array(ring_buffer_t *buffer, uint8_t *elements, ring_buffer_size_t size);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
