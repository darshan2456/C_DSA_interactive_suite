#include <assert.h>
#include <stdio.h>

/* Functions under test. They have external linkage but are not exposed in
   sorting_algorithms_n2.h, so we forward-declare them here (same approach the
   other test files use). All three share the (arr, length) signature. */
void bubble_sort_optimized(int arr[], int length_of_array);
void insertion_sort(int arr[], int length_of_array);
void selection_sort(int arr[], int length_of_array);

void test_bubble_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    bubble_sort_optimized(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    bubble_sort_optimized(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[5] = {1, 2, 3, 4, 5};
    bubble_sort_optimized(asc, 5);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);

    /* sorted descending (reverse order) */
    int desc[5] = {5, 4, 3, 2, 1};
    bubble_sort_optimized(desc, 5);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);

    /* duplicate values */
    int dup[7] = {4, 1, 4, 2, 1, 3, 4};
    bubble_sort_optimized(dup, 7);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 3);
    assert(dup[4] == 4);
    assert(dup[5] == 4);
    assert(dup[6] == 4);

    /* random values (including negatives) */
    int random_vals[8] = {9, -3, 0, 7, -10, 5, 2, -3};
    bubble_sort_optimized(random_vals, 8);
    assert(random_vals[0] == -10);
    assert(random_vals[1] == -3);
    assert(random_vals[2] == -3);
    assert(random_vals[3] == 0);
    assert(random_vals[4] == 2);
    assert(random_vals[5] == 5);
    assert(random_vals[6] == 7);
    assert(random_vals[7] == 9);

    printf("Bubble sort tests passed\n");
}

void test_insertion_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    insertion_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    insertion_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[5] = {1, 2, 3, 4, 5};
    insertion_sort(asc, 5);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);

    /* sorted descending (reverse order) */
    int desc[5] = {5, 4, 3, 2, 1};
    insertion_sort(desc, 5);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);

    /* duplicate values */
    int dup[7] = {4, 1, 4, 2, 1, 3, 4};
    insertion_sort(dup, 7);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 3);
    assert(dup[4] == 4);
    assert(dup[5] == 4);
    assert(dup[6] == 4);

    /* random values (including negatives) */
    int random_vals[8] = {9, -3, 0, 7, -10, 5, 2, -3};
    insertion_sort(random_vals, 8);
    assert(random_vals[0] == -10);
    assert(random_vals[1] == -3);
    assert(random_vals[2] == -3);
    assert(random_vals[3] == 0);
    assert(random_vals[4] == 2);
    assert(random_vals[5] == 5);
    assert(random_vals[6] == 7);
    assert(random_vals[7] == 9);

    printf("Insertion sort tests passed\n");
}

void test_selection_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    selection_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    selection_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[5] = {1, 2, 3, 4, 5};
    selection_sort(asc, 5);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);

    /* sorted descending (reverse order) */
    int desc[5] = {5, 4, 3, 2, 1};
    selection_sort(desc, 5);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);

    /* duplicate values */
    int dup[7] = {4, 1, 4, 2, 1, 3, 4};
    selection_sort(dup, 7);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 3);
    assert(dup[4] == 4);
    assert(dup[5] == 4);
    assert(dup[6] == 4);

    /* random values (including negatives) */
    int random_vals[8] = {9, -3, 0, 7, -10, 5, 2, -3};
    selection_sort(random_vals, 8);
    assert(random_vals[0] == -10);
    assert(random_vals[1] == -3);
    assert(random_vals[2] == -3);
    assert(random_vals[3] == 0);
    assert(random_vals[4] == 2);
    assert(random_vals[5] == 5);
    assert(random_vals[6] == 7);
    assert(random_vals[7] == 9);

    printf("Selection sort tests passed\n");
}

int main()
{
    test_bubble_sort();
    test_insertion_sort();
    test_selection_sort();
    printf("All O(n^2) sorting tests passed\n");
    return 0;
}
