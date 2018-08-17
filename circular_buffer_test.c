#include <stdio.h>

#include "circular_buffer.h"


circular_buffer_t circular_buffer;

unsigned int circular_buffer_test_count;

unsigned int circular_buffer_test_success_count;

unsigned int circular_buffer_overrun_callback_count;

unsigned int circular_buffer_underrun_callback_count;

void circular_buffer_test_setup()
{
    circular_buffer.head_index = 0;
    circular_buffer.tail_index = 0;

    circular_buffer_overrun_callback_count = 0;
    circular_buffer_underrun_callback_count = 0;
}

void circular_buffer_test_success(char *string)
{
    circular_buffer_test_count++;
    circular_buffer_test_success_count++;

    printf("[%i / %i] test success: %s\n", circular_buffer_test_success_count, circular_buffer_test_count, string);
}

void circular_buffer_test_fail(char *string)
{
    circular_buffer_test_count++;

    printf("[%i / %i] test fail: %s\n", circular_buffer_test_success_count, circular_buffer_test_count, string);
}

void circular_buffer_test_push_pop_element()
{
    circular_buffer_test_setup();

    char *identifier = "circular_buffer_test_push_pop_element";

    uint8_t element1 = 1;
    uint8_t element2 = 0;

    circular_buffer_status_t push_status = circular_buffer_push_element(&circular_buffer, element1);
    circular_buffer_status_t pop_status = circular_buffer_pop_element(&circular_buffer, &element2);

    if ((push_status != CIRCULAR_BUFFER_STATUS_OK) || (pop_status != CIRCULAR_BUFFER_STATUS_OK))
    {
        circular_buffer_test_fail(identifier);

        return;
    }

    if (element1 != element2)
    {
        circular_buffer_test_fail(identifier);

        return;
    }

    circular_buffer_test_success(identifier);
}

void circular_buffer_test_push_pop_elements()
{
    circular_buffer_test_setup();

    char *identifier = "circular_buffer_test_push_pop_elements";

    uint8_t elements1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8_t elements2[16];

    circular_buffer_size_t element_count = 16;

    circular_buffer_size_t element_count1 = circular_buffer_push_elements(&circular_buffer, elements1, element_count);
    circular_buffer_size_t element_count2 = circular_buffer_pop_elements(&circular_buffer, elements2, element_count);

    if ((element_count1 != element_count) || (element_count2 != element_count))
    {
        circular_buffer_test_fail(identifier);

        return;
    }

    for (size_t element_index = 0; element_index < element_count; element_index++)
    {
        if (elements1[element_index] != elements2[element_index])
        {
            circular_buffer_test_fail(identifier);

            return;
        }
    }

    circular_buffer_test_success(identifier);
}

void circular_buffer_test_overrun_callback()
{
    circular_buffer_test_setup();

    char *identifier = "circular_buffer_test_overrun_callback";

    uint8_t elements[CIRCULAR_BUFFER_SIZE + 2];

    circular_buffer_size_t element_count =
        circular_buffer_push_elements(&circular_buffer, elements, CIRCULAR_BUFFER_SIZE + 2);

    if ((element_count != CIRCULAR_BUFFER_SIZE - 1) || (circular_buffer_overrun_callback_count != 1))
    {
        circular_buffer_test_fail(identifier);

        return;
    }

    circular_buffer_test_success(identifier);
}

void circular_buffer_test_underrun_callback()
{
    circular_buffer_test_setup();

    char *identifier = "circular_buffer_test_underrun_callback";

    uint8_t elements[2];

    circular_buffer_size_t element_count =
        circular_buffer_pop_elements(&circular_buffer, elements, 2);

    if ((element_count != 0) || (circular_buffer_underrun_callback_count != 1))
    {
        circular_buffer_test_fail(identifier);

        return;
    }

    circular_buffer_test_success(identifier);
}

void circular_buffer_overrun_callback(circular_buffer_t *buffer)
{
    circular_buffer_overrun_callback_count++;
}

void circular_buffer_underrun_callback(circular_buffer_t *buffer)
{
    circular_buffer_underrun_callback_count++;
}

int main()
{
    circular_buffer_test_push_pop_element();
    circular_buffer_test_push_pop_elements();
    circular_buffer_test_overrun_callback();
    circular_buffer_test_underrun_callback();

    return 0;
}
