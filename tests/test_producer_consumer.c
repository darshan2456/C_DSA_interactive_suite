#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/clear_screen.h"
#include "../src/utils/cross_platform_timer.h"
#include "../src/utils/config.h"

#define TEST_MOCK_SYNC
#define is_instant() 1
#define clear_screen() (void)0
#define sleep_seconds(x) (void)0
#define getchar() ((void)'\n')

#include "mock_printf.h"

// Mocking safe_input_int to feed inputs to the interactive loop
static int g_inputs[100];
static int g_inputs_count = 0;
static int g_inputs_idx = 0;

void set_inputs(const int* vals, int count)
{
    g_inputs_count = count;
    g_inputs_idx = 0;
    for (int i = 0; i < count; i++)
    {
        g_inputs[i] = vals[i];
    }
}

int mock_safe_input_int(int* var, const char* prompt, int low, int high)
{
    (void)prompt;
    (void)low;
    (void)high;
    if (g_inputs_idx < g_inputs_count)
    {
        *var = g_inputs[g_inputs_idx++];
        return 1; // Success
    }
    *var = -1;
    return -111; // INPUT_EXIT_SIGNAL
}

// Redirect functions using preprocessor macros
#define safe_input_int mock_safe_input_int
#define printf mock_printf
#include "../src/process_synchronization/producer_consumer.c"
#undef safe_input_int
#undef printf

void test_producer_consumer_basic()
{
    reset_printf_buf();

    // Inputs: Produce (1), Produce (1), Consume (2), Exit (-1)
    int inputs[] = {1, 1, 2, -1};
    set_inputs(inputs, 4);

    producer_consumer_demo();

    // Verify item production and consumption occurred
    assert(strstr(g_printf_buf, "placed item P1") != NULL);
    assert(strstr(g_printf_buf, "placed item P2") != NULL);
    assert(strstr(g_printf_buf, "consumed item P1") != NULL);
}

void test_producer_consumer_full_blocking()
{
    reset_printf_buf();

    // Inputs: Produce 5 times (fills buffer), then Produce 6th time (blocks), Exit (-1)
    int inputs[] = {1, 1, 1, 1, 1, 1, -1};
    set_inputs(inputs, 7);

    producer_consumer_demo();

    // Verify buffer full and blocking occurred
    assert(strstr(g_printf_buf, "placed item P5") != NULL);
    assert(strstr(g_printf_buf, "Producer is BLOCKED") != NULL);
}

void test_producer_consumer_empty_blocking()
{
    reset_printf_buf();

    // Inputs: Consume when empty (blocks), Exit (-1)
    int inputs[] = {2, -1};
    set_inputs(inputs, 2);

    producer_consumer_demo();

    // Verify buffer empty and blocking occurred
    assert(strstr(g_printf_buf, "Consumer is BLOCKED") != NULL);
}

int main()
{
    test_producer_consumer_basic();
    test_producer_consumer_full_blocking();
    test_producer_consumer_empty_blocking();

    fprintf(stdout, "All Producer-Consumer tests passed\n");
    return 0;
}
