#ifndef ADVANCED_SORTING_H
#define ADVANCED_SORTING_H

#include "sorting_telemetry.h"

void quicksort(int arr[], int low, int high);
void quicksort_with_telemetry(int arr[], int low, int high, SortingTelemetry* telemetry);
void quicksort_demo(void);
void advanced_sorting_demo(void);
void merge_sort(int arr[], int n);
void merge_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry);
void merge_sort_demo(void);
void heap_sort(int arr[], int n);
void heap_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry);
void heap_sort_demo(void);
void radix_sort(int arr[], int n);
void radix_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry);
void radix_sort_demo(void);
void bucket_sort(int arr[], int n);
void bucket_sort_demo(void);

#endif
