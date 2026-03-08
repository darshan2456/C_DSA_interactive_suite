#include "safe_input.h"
#include <stdio.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[low];
    int left = low + 1;
    int right = high;

    while (1)
    {
        while (left <= right && arr[left] <= pivot)
        {
            left++;
        }
        while (arr[right] > pivot)
        {
            right--;
        }
        if (left >= right)
            break;

        swap(&arr[left], &arr[right]);
    }
    swap(&arr[low], &arr[right]);

    return right;
}

void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        int p = partition(arr, low, high);
        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

void quicksort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nQuicksort demo");
        int quicksort_status = safe_input_int(&length_of_array,
                                              "\nenter the number of elements in the array "
                                              "(between 1 and 100), enter '-1' to exit:- ",
                                              1, 100);

        if (quicksort_status == 0)
            continue; // retry on failure

        if (quicksort_status == INPUT_EXIT_SIGNAL)
        { // exit condition, user enters '-1'
            printf("\nExiting quicksort demo....\n");
            return;
        }

        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {

        retry:
            printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting quicksort demo.....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        quicksort(arr, 0, length_of_array - 1);

        printf("\nThe array sorted by quicksort is:- ");
        for (int i = 0; i < length_of_array; i++)
        {
            printf("%d", arr[i]);
            if (i < length_of_array - 1)
                printf(",");
        }
    }
}
