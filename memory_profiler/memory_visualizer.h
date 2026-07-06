#ifndef MEMORY_VISUALIZER_H
#define MEMORY_VISUALIZER_H

#include <stddef.h>

void print_live_memory_card(void);
void print_ascii_memory_graph(size_t* values, int count);
void compare_algorithms_memory(const char* algo1_name, void (*algo1_func)(void*),
                               const char* algo2_name, void (*algo2_func)(void*), void* input_arg);

#endif // MEMORY_VISUALIZER_H
