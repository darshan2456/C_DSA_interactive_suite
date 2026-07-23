#include "bit_manipulation.h"

/**
 * Sets the kth bit of a number n.
 * Assumes k is 0-indexed.
 */
int set_bit(int n, int k)
{
    return (n | (1U << k));
}

/**
 * Clears the kth bit of a number n.
 * Assumes k is 0-indexed.
 */
int clear_bit(int n, int k)
{
    return (n & (~(1U << k)));
}

/**
 * Toggles the kth bit of a number n.
 * Assumes k is 0-indexed.
 */
int toggle_bit(int n, int k)
{
    return (n ^ (1U << k));
}

/**
 * Checks if the kth bit of a number n is set.
 * Assumes k is 0-indexed.
 * Returns 1 if set, 0 otherwise.
 */
int check_bit(int n, int k)
{
    return ((n & (1U << k)) != 0) ? 1 : 0;
}
