#include "error_correction_algorithms.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_parity_invalid_inputs(void)
{
    // NULL input
    assert(generateEvenParity(NULL) == -1);
    assert(generateOddParity(NULL) == -1);
    assert(verifyEvenParity(NULL) == -1);
    assert(verifyOddParity(NULL) == -1);

    // Empty input
    assert(generateEvenParity("") == -1);
    assert(generateOddParity("") == -1);
    assert(verifyEvenParity("") == -1);
    assert(verifyOddParity("") == -1);

    // Non-binary characters
    assert(generateEvenParity("102") == -1);
    assert(generateOddParity("abc") == -1);
    assert(verifyEvenParity("10 1") == -1);
    assert(verifyOddParity("101a") == -1);

    printf("test_parity_invalid_inputs passed\n");
}

void test_parity_generation(void)
{
    // Even number of 1s (0 ones) -> even parity bit is 0, odd parity bit is 1
    assert(generateEvenParity("0") == 0);
    assert(generateOddParity("0") == 1);

    assert(generateEvenParity("0000") == 0);
    assert(generateOddParity("0000") == 1);

    // Odd number of 1s (1 one) -> even parity bit is 1, odd parity bit is 0
    assert(generateEvenParity("1") == 1);
    assert(generateOddParity("1") == 0);

    assert(generateEvenParity("010") == 1);
    assert(generateOddParity("010") == 0);

    // Complex strings
    // "1101001111" has 7 ones (odd)
    assert(generateEvenParity("1101001111") == 1);
    assert(generateOddParity("1101001111") == 0);

    // "11010011111" has 8 ones (even)
    assert(generateEvenParity("11010011111") == 0);
    assert(generateOddParity("11010011111") == 1);

    printf("test_parity_generation passed\n");
}

void test_parity_verification_and_errors(void)
{
    // We simulate transmission and verification
    const char* data1 = "1101001111";        // 7 ones
    int p_even1 = generateEvenParity(data1); // Should be 1
    int p_odd1 = generateOddParity(data1);   // Should be 0

    assert(p_even1 == 1);
    assert(p_odd1 == 0);

    // Append parity bit to form received data
    char transmitted_even1[32];
    char transmitted_odd1[32];
    snprintf(transmitted_even1, sizeof(transmitted_even1), "%s%d", data1, p_even1);
    snprintf(transmitted_odd1, sizeof(transmitted_odd1), "%s%d", data1, p_odd1);

    // Verify correct transmission
    assert(verifyEvenParity(transmitted_even1) == 1);
    assert(verifyOddParity(transmitted_odd1) == 1);

    // Simulate single-bit transmission errors
    // Flip first bit: "11010011111" -> "01010011111"
    char corrupt_even[32];
    strcpy(corrupt_even, transmitted_even1);
    corrupt_even[0] = (corrupt_even[0] == '1') ? '0' : '1';
    assert(verifyEvenParity(corrupt_even) == 0); // Should detect error

    // Flip parity bit
    strcpy(corrupt_even, transmitted_even1);
    corrupt_even[strlen(corrupt_even) - 1] =
        (corrupt_even[strlen(corrupt_even) - 1] == '1') ? '0' : '1';
    assert(verifyEvenParity(corrupt_even) == 0); // Should detect error

    // Odd parity corruptions
    char corrupt_odd[32];
    strcpy(corrupt_odd, transmitted_odd1);
    corrupt_odd[3] = (corrupt_odd[3] == '1') ? '0' : '1';
    assert(verifyOddParity(corrupt_odd) == 0); // Should detect error

    printf("test_parity_verification_and_errors passed\n");
}

void test_parity_multiple_lengths(void)
{
    // Test multiple lengths
    char data[200];
    for (int len = 1; len < 150; len++)
    {
        // fill with '1's
        for (int i = 0; i < len; i++)
        {
            data[i] = '1';
        }
        data[len] = '\0';

        int p_even = generateEvenParity(data);
        int expected_even = (len % 2 == 0) ? 0 : 1;
        assert(p_even == expected_even);

        int p_odd = generateOddParity(data);
        int expected_odd = (len % 2 == 0) ? 1 : 0;
        assert(p_odd == expected_odd);
    }
    printf("test_parity_multiple_lengths passed\n");
}

int main(void)
{
    test_parity_invalid_inputs();
    test_parity_generation();
    test_parity_verification_and_errors();
    test_parity_multiple_lengths();

    printf("All parity bit tests passed successfully!\n");
    return 0;
}
