#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdbool.h>

// RLE Compression Algorithms
int rle_encode(const char* input, char* output, int out_max);
int rle_decode(const char* input, char* output, int out_max);

// Main Demo Menu
void compression_demo(void);

#endif // COMPRESSION_H
