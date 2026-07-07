#include "step_debugger.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

extern void bubble_sort_optimized(int arr[], int length_of_array);

void test_debugger_state(void)
{
    set_step_mode(1);
    assert(get_step_mode() == 1);

    set_step_mode(0);
    assert(get_step_mode() == 0);

    set_paused(1);
    assert(get_paused() == 1);

    set_paused(0);
    assert(get_paused() == 0);

    printf("test_debugger_state passed.\n");
}

void test_debugger_events(void)
{
    clear_recent_events();

    char events[5][128];
    int count = 0;
    get_recent_events(events, &count);
    assert(count == 0);

    algorithm_step_hook("Event A");
    algorithm_step_hook("Event B");

    get_recent_events(events, &count);
    assert(count == 2);
    assert(strcmp(events[0], "Event A") == 0);
    assert(strcmp(events[1], "Event B") == 0);

    // Test overflow
    algorithm_step_hook("Event C");
    algorithm_step_hook("Event D");
    algorithm_step_hook("Event E");
    algorithm_step_hook("Event F");

    get_recent_events(events, &count);
    assert(count == 5);
    assert(strcmp(events[0], "Event B") == 0);
    assert(strcmp(events[4], "Event F") == 0);

    print_recent_events_card();

    printf("test_debugger_events passed.\n");
}

void test_debugger_step_counting(void)
{
    clear_recent_events();
    set_step_mode(1); // Set active to track events, but keep paused=0 to prevent blocking
    set_paused(0);

    int arr[] = {1, 2, 3};
    bubble_sort_optimized(arr, 3);

    char events[5][128];
    int count = 0;
    get_recent_events(events, &count);

    // The optimized Bubble Sort makes exactly 2 comparisons on a sorted array of size 3 before
    // terminating.
    assert(count == 2);
    assert(strstr(events[0], "Comparing index 0") != NULL);
    assert(strstr(events[1], "Comparing index 1") != NULL);

    set_step_mode(0);
    printf("test_debugger_step_counting passed.\n");
}

int main(void)
{
    printf("Starting Step Debugger unit tests...\n");
    test_debugger_state();
    test_debugger_events();
    test_debugger_step_counting();
    printf("All Step Debugger unit tests passed successfully!\n");
    return 0;
}
