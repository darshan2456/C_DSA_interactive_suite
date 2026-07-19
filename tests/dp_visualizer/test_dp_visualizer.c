#include "dp_visualizer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Mock printf to suppress output
int mock_printf(const char* format, ...)
{
    (void)format;
    return 0;
}

#define printf mock_printf
#include "../../features/dp_visualizer/dp_visualizer.c"
#undef printf

void test_visualize_1d()
{
    long long table[5] = {0, 1, 1, 2, 3};
    // Should execute without errors or crashes
    visualize_dp_table_1d("Test 1D", table, 5, 2);
    visualize_dp_table_1d("Test 1D Large", table, 5,
                          10); // out of bounds active_index should handle gracefully
}

void test_visualize_2d()
{
    int rows = 3, cols = 3;
    int** table = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        table[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
        {
            table[i][j] = i + j;
        }
    }

    // Should execute without errors or crashes
    visualize_dp_table_2d("Test 2D", table, rows, cols, "abc", "xyz", 1, 1);

    for (int i = 0; i < rows; i++)
    {
        free(table[i]);
    }
    free(table);
}

void test_visualize_mcm()
{
    int n = 4;
    int m[16] = {0};
    int s[16] = {0};
    visualize_mcm_tables(m, s, n, 1, 2);
}

int main()
{
    test_visualize_1d();
    test_visualize_2d();
    test_visualize_mcm();
    return 0;
}
