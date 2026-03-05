#include <assert.h>
#include <stdio.h>

/* Forward declarations */
int linear_search(int arr[], int target, int length_of_array);
int binary_search(int arr[], int target, int length_of_array);

void test_linear_search()
{

    int arr1[] = {1, 2, 3, 4, 5};
    assert(linear_search(arr1, 1, 5) == 0);
    assert(linear_search(arr1, 5, 5) == 4);
    assert(linear_search(arr1, 3, 5) == 2);
    assert(linear_search(arr1, 10, 5) == -1);

    int arr2[] = {42};
    assert(linear_search(arr2, 42, 1) == 0);
    assert(linear_search(arr2, 1, 1) == -1);

    int arr3[] = {5, 5, 5};
    assert(linear_search(arr3, 5, 3) == 0); // first occurrence

    printf("Linear search tests passed\n");
}

void test_binary_search()
{

    int arr1[] = {1, 2, 3, 4, 5};
    assert(binary_search(arr1, 1, 5) == 0);
    assert(binary_search(arr1, 5, 5) == 4);
    assert(binary_search(arr1, 3, 5) == 2);
    assert(binary_search(arr1, 10, 5) == -1);

    int arr2[] = {42};
    assert(binary_search(arr2, 42, 1) == 0);
    assert(binary_search(arr2, 1, 1) == -1);

    int arr3[] = {2, 4, 6, 8, 10, 12};
    assert(binary_search(arr3, 8, 6) == 3);

    printf("Binary search tests passed\n");
}

int main()
{
    test_linear_search();
    test_binary_search();
    printf("All search tests passed\n");
    return 0;
}