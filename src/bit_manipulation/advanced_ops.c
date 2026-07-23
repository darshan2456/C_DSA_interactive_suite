#include "bit_manipulation.h"

/**
 * Counts the number of set bits (1s) in an integer using Brian Kernighan's Algorithm.
 * Time complexity: O(set bits)
 */
int count_set_bits(int n)
{
    int count = 0;
    while (n)
    {
        n = n & (n - 1);
        count++;
    }
    return count;
}

/**
 * Checks if a given integer is a power of 2.
 * Returns 1 if true, 0 otherwise.
 */
int is_power_of_two(int n)
{
    return (n > 0) && ((n & (n - 1)) == 0);
}

/**
 * Swaps two integers in place using the XOR operator without a temporary variable.
 */
void xor_swap(int* a, int* b)
{
    if (a != b)
    {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

/**
 * Returns the rightmost set bit of an integer.
 * This is effectively isolating the lowest set bit.
 */
int get_rightmost_set_bit(int n)
{
    return n & -n;
}

/**
 * Turns off (clears) the rightmost set bit of an integer.
 */
int turn_off_rightmost_set_bit(int n)
{
    return n & (n - 1);
}

/**
 * Reverses the bits of a 32-bit unsigned integer.
 */
unsigned int reverse_bits(unsigned int n)
{
    unsigned int res = 0;
    for (int i = 0; i < 32; i++)
    {
        res <<= 1;
        if (n & 1)
        {
            res |= 1;
        }
        n >>= 1;
    }
    return res;
}
