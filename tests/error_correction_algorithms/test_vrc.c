#include "error_correction_algorithms.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* VRC (Vertical Redundancy Check) logic lifted directly from vrc.c/vrc_receiver.c.
   Since VRC has no extracted library helper, we replicate the logic here inline. */

static int compute_parity_bit(const char* data)
{
    int ones = 0;
    for (int i = 0; data[i] != '\0'; i++)
    {
        if (data[i] == '1')
            ones++;
    }
    return (ones % 2 == 0) ? 0 : 1; // Even parity
}

static int verify_frame(const char* frame)
{
    int ones = 0;
    for (int i = 0; frame[i] != '\0'; i++)
    {
        if (frame[i] == '1')
            ones++;
    }
    return (ones % 2 == 0) ? 1 : 0; // Even parity: total 1s must be even
}

void test_vrc_even_parity(void)
{
    // "1011" has 3 ones -> odd -> parity bit = 1
    assert(compute_parity_bit("1011") == 1);
    // Transmitted frame: "10111"
    assert(verify_frame("10111") == 1); // Even total 1s: 4 -> clean

    // "1100" has 2 ones -> even -> parity bit = 0
    assert(compute_parity_bit("1100") == 0);
    // Transmitted frame: "11000"
    assert(verify_frame("11000") == 1); // Even total 1s: 2 -> clean

    printf("test_vrc_even_parity passed\n");
}

void test_vrc_error_detection(void)
{
    // "1011" + parity -> "10111", flip bit 0 -> "00111" (3 ones, odd -> error)
    assert(verify_frame("00111") == 0); // Error detected

    // "1100" + parity -> "11000", flip bit 2 -> "11100" (3 ones, odd -> error)
    assert(verify_frame("11100") == 0); // Error detected

    printf("test_vrc_error_detection passed\n");
}

void test_vrc_all_zeros(void)
{
    // "0000" has 0 ones -> even -> parity bit = 0
    assert(compute_parity_bit("0000") == 0);
    // Transmitted frame: "00000" -> 0 ones, even -> clean
    assert(verify_frame("00000") == 1);

    printf("test_vrc_all_zeros passed\n");
}

void test_vrc_all_ones(void)
{
    // "1111" has 4 ones -> even -> parity bit = 0
    assert(compute_parity_bit("1111") == 0);
    // Transmitted frame: "11110" -> 4 ones, even -> clean
    assert(verify_frame("11110") == 1);

    printf("test_vrc_all_ones passed\n");
}

int main(void)
{
    test_vrc_even_parity();
    test_vrc_error_detection();
    test_vrc_all_zeros();
    test_vrc_all_ones();

    printf("All VRC tests passed successfully!\n");
    return 0;
}
