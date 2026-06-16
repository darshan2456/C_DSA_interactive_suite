#include <assert.h>
#include <limits.h>
#include <stdio.h>

/* Functions under test (forward-declared, same approach as the other tests).
   quicksort takes (arr, low, high); the rest take (arr, n). radix_sort only
   supports non-negative integers, so every case below uses non-negative data
   to keep one consistent set of expectations across all four algorithms. */
void quicksort(int arr[], int low, int high);
void merge_sort(int arr[], int n);
void heap_sort(int arr[], int n);
void radix_sort(int arr[], int n);
void bucket_sort(int arr[], int n);

void test_quick_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    quicksort(empty, 0, -1);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    quicksort(single, 0, 0);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[6] = {1, 2, 3, 4, 5, 6};
    quicksort(asc, 0, 5);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);
    assert(asc[5] == 6);

    /* sorted descending (reverse order) */
    int desc[6] = {6, 5, 4, 3, 2, 1};
    quicksort(desc, 0, 5);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);
    assert(desc[5] == 6);

    /* duplicate values */
    int dup[8] = {5, 1, 5, 2, 1, 3, 5, 2};
    quicksort(dup, 0, 7);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 2);
    assert(dup[4] == 3);
    assert(dup[5] == 5);
    assert(dup[6] == 5);
    assert(dup[7] == 5);

    /* random values (multi-digit) */
    int random_vals[9] = {170, 45, 75, 90, 2, 802, 24, 66, 1};
    quicksort(random_vals, 0, 8);
    assert(random_vals[0] == 1);
    assert(random_vals[1] == 2);
    assert(random_vals[2] == 24);
    assert(random_vals[3] == 45);
    assert(random_vals[4] == 66);
    assert(random_vals[5] == 75);
    assert(random_vals[6] == 90);
    assert(random_vals[7] == 170);
    assert(random_vals[8] == 802);

    printf("Quick sort tests passed\n");
}

void test_merge_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    merge_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    merge_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[6] = {1, 2, 3, 4, 5, 6};
    merge_sort(asc, 6);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);
    assert(asc[5] == 6);

    /* sorted descending (reverse order) */
    int desc[6] = {6, 5, 4, 3, 2, 1};
    merge_sort(desc, 6);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);
    assert(desc[5] == 6);

    /* duplicate values */
    int dup[8] = {5, 1, 5, 2, 1, 3, 5, 2};
    merge_sort(dup, 8);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 2);
    assert(dup[4] == 3);
    assert(dup[5] == 5);
    assert(dup[6] == 5);
    assert(dup[7] == 5);

    /* random values (multi-digit) */
    int random_vals[9] = {170, 45, 75, 90, 2, 802, 24, 66, 1};
    merge_sort(random_vals, 9);
    assert(random_vals[0] == 1);
    assert(random_vals[1] == 2);
    assert(random_vals[2] == 24);
    assert(random_vals[3] == 45);
    assert(random_vals[4] == 66);
    assert(random_vals[5] == 75);
    assert(random_vals[6] == 90);
    assert(random_vals[7] == 170);
    assert(random_vals[8] == 802);

    printf("Merge sort tests passed\n");
}

void test_heap_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    heap_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    heap_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[6] = {1, 2, 3, 4, 5, 6};
    heap_sort(asc, 6);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);
    assert(asc[5] == 6);

    /* sorted descending (reverse order) */
    int desc[6] = {6, 5, 4, 3, 2, 1};
    heap_sort(desc, 6);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);
    assert(desc[5] == 6);

    /* duplicate values */
    int dup[8] = {5, 1, 5, 2, 1, 3, 5, 2};
    heap_sort(dup, 8);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 2);
    assert(dup[4] == 3);
    assert(dup[5] == 5);
    assert(dup[6] == 5);
    assert(dup[7] == 5);

    /* random values (multi-digit) */
    int random_vals[9] = {170, 45, 75, 90, 2, 802, 24, 66, 1};
    heap_sort(random_vals, 9);
    assert(random_vals[0] == 1);
    assert(random_vals[1] == 2);
    assert(random_vals[2] == 24);
    assert(random_vals[3] == 45);
    assert(random_vals[4] == 66);
    assert(random_vals[5] == 75);
    assert(random_vals[6] == 90);
    assert(random_vals[7] == 170);
    assert(random_vals[8] == 802);

    printf("Heap sort tests passed\n");
}

void test_radix_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    radix_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    radix_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[6] = {1, 2, 3, 4, 5, 6};
    radix_sort(asc, 6);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);
    assert(asc[5] == 6);

    /* sorted descending (reverse order) */
    int desc[6] = {6, 5, 4, 3, 2, 1};
    radix_sort(desc, 6);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);
    assert(desc[5] == 6);

    /* duplicate values */
    int dup[8] = {5, 1, 5, 2, 1, 3, 5, 2};
    radix_sort(dup, 8);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 2);
    assert(dup[4] == 3);
    assert(dup[5] == 5);
    assert(dup[6] == 5);
    assert(dup[7] == 5);

    /* random values (multi-digit, exercises multiple radix passes) */
    int random_vals[9] = {170, 45, 75, 90, 2, 802, 24, 66, 1};
    radix_sort(random_vals, 9);
    assert(random_vals[0] == 1);
    assert(random_vals[1] == 2);
    assert(random_vals[2] == 24);
    assert(random_vals[3] == 45);
    assert(random_vals[4] == 66);
    assert(random_vals[5] == 75);
    assert(random_vals[6] == 90);
    assert(random_vals[7] == 170);
    assert(random_vals[8] == 802);

    printf("Radix sort tests passed\n");
}

void test_bucket_sort()
{
    /* empty array: the sort must touch nothing */
    int empty[1] = {42};
    bucket_sort(empty, 0);
    assert(empty[0] == 42);

    /* single element */
    int single[1] = {7};
    bucket_sort(single, 1);
    assert(single[0] == 7);

    /* already sorted ascending */
    int asc[6] = {1, 2, 3, 4, 5, 6};
    bucket_sort(asc, 6);
    assert(asc[0] == 1);
    assert(asc[1] == 2);
    assert(asc[2] == 3);
    assert(asc[3] == 4);
    assert(asc[4] == 5);
    assert(asc[5] == 6);

    /* sorted descending (reverse order) */
    int desc[6] = {6, 5, 4, 3, 2, 1};
    bucket_sort(desc, 6);
    assert(desc[0] == 1);
    assert(desc[1] == 2);
    assert(desc[2] == 3);
    assert(desc[3] == 4);
    assert(desc[4] == 5);
    assert(desc[5] == 6);

    /* duplicate values */
    int dup[8] = {5, 1, 5, 2, 1, 3, 5, 2};
    bucket_sort(dup, 8);
    assert(dup[0] == 1);
    assert(dup[1] == 1);
    assert(dup[2] == 2);
    assert(dup[3] == 2);
    assert(dup[4] == 3);
    assert(dup[5] == 5);
    assert(dup[6] == 5);
    assert(dup[7] == 5);

    /* negative numbers (bucket sort handles them) */
    int neg[6] = {-3, 5, -1, 0, -10, 2};
    bucket_sort(neg, 6);
    assert(neg[0] == -10);
    assert(neg[1] == -3);
    assert(neg[2] == -1);
    assert(neg[3] == 0);
    assert(neg[4] == 2);
    assert(neg[5] == 5);

    /* random values (multi-digit) */
    int random_vals[9] = {170, 45, 75, 90, 2, 802, 24, 66, 1};
    bucket_sort(random_vals, 9);
    assert(random_vals[0] == 1);
    assert(random_vals[1] == 2);
    assert(random_vals[2] == 24);
    assert(random_vals[3] == 45);
    assert(random_vals[4] == 66);
    assert(random_vals[5] == 75);
    assert(random_vals[6] == 90);
    assert(random_vals[7] == 170);
    assert(random_vals[8] == 802);

    /* wide value range: max - min exceeds INT_MAX, so the bucket-index
       divisor must be computed in a wider type to avoid signed overflow
       and an out-of-bounds bucket index */
    int wide[3] = {INT_MAX, 0, INT_MIN};
    bucket_sort(wide, 3);
    assert(wide[0] == INT_MIN);
    assert(wide[1] == 0);
    assert(wide[2] == INT_MAX);

    printf("Bucket sort tests passed\n");
}

int main()
{
    test_quick_sort();
    test_merge_sort();
    test_heap_sort();
    test_radix_sort();
    test_bucket_sort();
    printf("All advanced sorting tests passed\n");
    return 0;
}
