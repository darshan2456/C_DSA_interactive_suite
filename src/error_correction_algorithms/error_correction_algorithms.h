#ifndef ERROR_CORRECTION_ALGORITHMS_H
#define ERROR_CORRECTION_ALGORITHMS_H

#include <stddef.h>

// longest binary string accepted from the user (plus room for the '\0'). a checksum
// demo never needs more than a few hundred bits, so a fixed buffer keeps things simple.
#define CHECKSUM_MAX_BITS 1024

void error_correction_algorithms_demo(void);
void checksum_demo(void);
void checksum_receiver_demo(void);
void crc_demo(void);
void lrc_demo(void);
void vrc_demo(void);
void vrc_receiver_demo(void);

// shared checksum helpers (defined in checksum.c, reused by the receiver side)
void checksum_print_binary(int value, int bits);
int checksum_read_binary(char* buff, size_t size, const char* prompt);
int checksum_add(int sum, int word, int k);
int checksum_block_sum(const char* data, int len, int k);

#endif
