#ifndef SAFE_INPUT_H
#define SAFE_INPUT_H

#define INPUT_EXIT_SIGNAL -111

int safe_input_int(int *input, const char *prompt, int min_val, int max_val);

#endif