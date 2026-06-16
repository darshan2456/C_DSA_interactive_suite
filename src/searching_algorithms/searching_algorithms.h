#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

int linear_search_demo(void);
int binary_search_demo(void);
void binary_search_recursive_demo(void);
void searching_algorithms_demo(void);
int linear_search(int arr[], int target, int length_of_array);
int binary_search(int arr[], int target, int length_of_array);
int binary_search_recursive(int arr[], int target, int low, int high);
int interpolation_search(int arr[], int target, int length_of_array);
void interpolation_search_demo(void);
int jump_search(int arr[], int target, int length_of_array);
void jump_search_demo(void);

#endif
