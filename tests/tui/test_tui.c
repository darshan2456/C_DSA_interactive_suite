#include <assert.h>
#include <stdio.h>

#define main tui_main_unused
#include "../../tui/tui.c"
#undef main

void test_build_visible(void)
{
    printf("Running test_build_visible...\n");
    int visible[256];
    int count = build_visible(visible, 256);
    assert(count > 0);
    // Make sure we have "Animation speed (s)" as first visible category
    assert(visible[0] == 0);
    printf("--> test_build_visible PASSED! (%d visible items)\n", count);
}

void test_find_parent(void)
{
    printf("Running test_find_parent...\n");
    // Find parent of first item
    int parent =
        find_parent(1); // Set Animation Speed parent should be "Animation speed (s)" (idx 0)
    assert(parent == 0);
    printf("--> test_find_parent PASSED!\n");
}

int main(void)
{
    printf("Starting TUI unit tests...\n");
    test_build_visible();
    test_find_parent();
    printf("All TUI unit tests passed successfully!\n");
    return 0;
}
