#include <assert.h>
#include <stdio.h>
#include "array.h"

/* swap tests */
void test_swap_adjacent_pairs() {
    int arr[] = {1,2,3,4};
    swap_adjacent_pairs(arr,4);

    assert(arr[0] == 2);
    assert(arr[1] == 1);
    assert(arr[2] == 4);
    assert(arr[3] == 3);

    printf("Array swap adjacent pairs test passed\n");
}

/* swap with odd length */
void test_swap_odd_length() {
    int arr[] = {1,2,3,4,5};
    swap_adjacent_pairs(arr,5);

    assert(arr[0] == 2);
    assert(arr[1] == 1);
    assert(arr[2] == 4);
    assert(arr[3] == 3);
    assert(arr[4] == 5);

    printf("Array swap odd length test passed\n");
}

/* reverse test */
void test_reverse_array() {
    int arr[] = {1,2,3,4};
    reverse_array(arr,4);

    assert(arr[0] == 4);
    assert(arr[1] == 3);
    assert(arr[2] == 2);
    assert(arr[3] == 1);

    printf("Array reverse test passed\n");
}

/* reverse single element */
void test_reverse_single() {
    int arr[] = {42};
    reverse_array(arr,1);

    assert(arr[0] == 42);

    printf("Array reverse single element test passed\n");
}

/* max element test */
void test_max_array() {
    int arr[] = {5,10,3,7};

    int max = max_array(arr,4);
    assert(max == 10);

    printf("Array max element test passed\n");
}

/* min element test */
void test_min_array() {
    int arr[] = {5,10,3,7};

    int min = min_array(arr,4);
    assert(min == 3);

    printf("Array min element test passed\n");
}

/* edge case with negatives */
void test_negative_values() {
    int arr[] = {-5,-1,-10,-3};

    assert(max_array(arr,4) == -1);
    assert(min_array(arr,4) == -10);

    printf("Array negative value test passed\n");
}

int main() {

    test_swap_adjacent_pairs();
    test_swap_odd_length();
    test_reverse_array();
    test_reverse_single();
    test_max_array();
    test_min_array();
    test_negative_values();

    printf("All array tests passed\n");

    return 0;
}