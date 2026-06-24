#include "sorting_algorithms_n2.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_shell_sort_basic()
{
    int arr[] = {12, 34, 54, 2, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    shell_sort(arr, n);

    assert(arr[0] == 2);
    assert(arr[1] == 3);
    assert(arr[2] == 12);
    assert(arr[3] == 34);
    assert(arr[4] == 54);

    printf("test_shell_sort_basic passed\n");
}

void test_shell_sort_duplicates()
{
    int arr[] = {4, 2, 4, 3, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    shell_sort(arr, n);

    assert(arr[0] == 2);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
    assert(arr[3] == 4);
    assert(arr[4] == 4);

    printf("test_shell_sort_duplicates passed\n");
}

void test_shell_sort_negatives()
{
    int arr[] = {-5, 10, -1, 0, -3};
    int n = sizeof(arr) / sizeof(arr[0]);

    shell_sort(arr, n);

    assert(arr[0] == -5);
    assert(arr[1] == -3);
    assert(arr[2] == -1);
    assert(arr[3] == 0);
    assert(arr[4] == 10);

    printf("test_shell_sort_negatives passed\n");
}

void test_shell_sort_single_element()
{
    int arr[] = {42};
    int n = sizeof(arr) / sizeof(arr[0]);

    shell_sort(arr, n);

    assert(arr[0] == 42);

    printf("test_shell_sort_single_element passed\n");
}

int main()
{
    test_shell_sort_basic();
    test_shell_sort_duplicates();
    test_shell_sort_negatives();
    test_shell_sort_single_element();

    printf("All Shell Sort tests passed\n");
    return 0;
}
