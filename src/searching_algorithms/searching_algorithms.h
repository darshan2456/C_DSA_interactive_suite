#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

// Run the linear search demonstration module. Returns The status code from the demo run.
int linear_search_demo(void);

// Run the binary search demonstration module. Returns The status code from the demo run.
int binary_search_demo(void);

// Run the recursive binary search demonstration module.
void binary_search_recursive_demo(void);

// Run the searching algorithms demonstration module.
void searching_algorithms_demo(void);

// Search for a value using linear search. Returns The index of the target or -1 if not found.
int linear_search(int arr[], int target, int length_of_array);

// Search for a value using binary search. Returns The index of the target or -1 if not found.
int binary_search(int arr[], int target, int length_of_array);

// Search for a value using recursive binary search. Returns The index of the target or -1 if not
// found.
int binary_search_recursive(int arr[], int target, int low, int high);

// Search for a value using interpolation search. Returns The index of the target or -1 if not
// found.
int interpolation_search(int arr[], int target, int length_of_array);

// Run the interpolation search demonstration module.
void interpolation_search_demo(void);

// Search for a value using jump search. Returns The index of the target or -1 if not found.
int jump_search(int arr[], int target, int length_of_array);

// Run the jump search demonstration module.
void jump_search_demo(void);

#endif
