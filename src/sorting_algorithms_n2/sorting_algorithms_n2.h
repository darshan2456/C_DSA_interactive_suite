#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "sorting_telemetry.h"

// Run the bubble sort optimized demonstration module.
void bubble_sort_optimized_demo(void);

// Run the insertion sort demonstration module.
void insertion_sort_demo(void);

// Run the selection sort demonstration module.
void selection_sort_demo(void);

// Run the shell sort demonstration module.
void shell_sort_demo(void);

// Run the sorting algorithms demonstration module.
void sorting_algorithms_n2_demo(void);

// Sort an integer array using selection sort.
void selection_sort(int arr[], int length_of_array);
void selection_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry);

// Sort an integer array using shell sort.
void shell_sort(int arr[], int length_of_array);
void shell_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry);

// Sort an integer array using an optimized bubble sort.
void bubble_sort_optimized(int arr[], int length_of_array);
void bubble_sort_optimized_with_telemetry(int arr[], int length_of_array,
                                          SortingTelemetry* telemetry);

// Sort an integer array using insertion sort.
void insertion_sort(int arr[], int length_of_array);
void insertion_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry);

#endif