#include <stdio.h>

#include "ring_buffer.h"

ring_buffer_t ring_buffer;

unsigned int ring_buffer_test_count;

unsigned int ring_buffer_test_success_count;

unsigned int ring_buffer_overrun_callback_count;

unsigned int ring_buffer_underrun_callback_count;

void ring_buffer_test_setup()
{
    ring_buffer.head_index = 0;
    ring_buffer.tail_index = 0;

    ring_buffer_overrun_callback_count = 0;
    ring_buffer_underrun_callback_count = 0;
}

void ring_buffer_test_success(char *string)
{
    ring_buffer_test_count++;
    ring_buffer_test_success_count++;

    printf("[%i / %i] test success: %s\n", ring_buffer_test_success_count, ring_buffer_test_count, string);
}

void ring_buffer_test_fail(char *string)
{
    ring_buffer_test_count++;

    printf("[%i / %i] test fail: %s\n", ring_buffer_test_success_count, ring_buffer_test_count, string);
}

void ring_buffer_test_push_pop_element()
{
    ring_buffer_test_setup();

    char *identifier = "ring_buffer_test_push_pop_element";

    uint8_t element1 = 1;
    uint8_t element2 = 0;

    ring_buffer_status_t push_status = ring_buffer_push(&ring_buffer, element1);
    ring_buffer_status_t pop_status = ring_buffer_pop(&ring_buffer, &element2);

    if ((push_status != RING_BUFFER_STATUS_OK) || (pop_status != RING_BUFFER_STATUS_OK))
    {
        ring_buffer_test_fail(identifier);

        return;
    }

    if (element1 != element2)
    {
        ring_buffer_test_fail(identifier);

        return;
    }

    ring_buffer_test_success(identifier);
}

void ring_buffer_test_push_pop_elements()
{
    ring_buffer_test_setup();

    char *identifier = "ring_buffer_test_push_pop_elements";

    uint8_t elements1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8_t elements2[16];

    ring_buffer_size_t element_count = 16;

    ring_buffer_size_t element_count1 = ring_buffer_push_array(&ring_buffer, elements1, element_count);
    ring_buffer_size_t element_count2 = ring_buffer_pop_array(&ring_buffer, elements2, element_count);

    if ((element_count1 != element_count) || (element_count2 != element_count))
    {
        ring_buffer_test_fail(identifier);

        return;
    }

    for (size_t element_index = 0; element_index < element_count; element_index++)
    {
        if (elements1[element_index] != elements2[element_index])
        {
            ring_buffer_test_fail(identifier);

            return;
        }
    }

    ring_buffer_test_success(identifier);
}

void ring_buffer_test_overrun_callback()
{
    ring_buffer_test_setup();

    char *identifier = "ring_buffer_test_overrun_callback";

    uint8_t elements[RING_BUFFER_SIZE + 2];

    ring_buffer_size_t element_count =
        ring_buffer_push_array(&ring_buffer, elements, RING_BUFFER_SIZE + 2);

    if ((element_count != RING_BUFFER_SIZE - 1) || (ring_buffer_overrun_callback_count != 1))
    {
        ring_buffer_test_fail(identifier);

        return;
    }

    ring_buffer_test_success(identifier);
}

void ring_buffer_test_underrun_callback()
{
    ring_buffer_test_setup();

    char *identifier = "ring_buffer_test_underrun_callback";

    uint8_t elements[2];

    ring_buffer_size_t element_count =
        ring_buffer_pop_array(&ring_buffer, elements, 2);

    if ((element_count != 0) || (ring_buffer_underrun_callback_count != 1))
    {
        ring_buffer_test_fail(identifier);

        return;
    }

    ring_buffer_test_success(identifier);
}

void ring_buffer_overrun_cb(ring_buffer_t *buffer)
{
    UNUSED(buffer);

    ring_buffer_overrun_callback_count++;
}

void ring_buffer_underrun_cb(ring_buffer_t *buffer)
{
    UNUSED(buffer);

    ring_buffer_underrun_callback_count++;
}

int main()
{
    ring_buffer_test_push_pop_element();
    ring_buffer_test_push_pop_elements();
    ring_buffer_test_overrun_callback();
    ring_buffer_test_underrun_callback();

    return 0;
}
