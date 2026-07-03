#include "error_correction_algorithms.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_checksum_add(void)
{
    // Test 3-bit checksum addition (k=3)
    // 111 + 001 = 1000 -> 000 + 1 = 001
    assert(checksum_add(7, 1, 3) == 1);
    assert(checksum_add(5, 3, 3) == 1);
    assert(checksum_add(4, 4, 3) == 1);

    // Test 8-bit checksum addition (k=8)
    assert(checksum_add(255, 1, 8) == 1);
    assert(checksum_add(100, 150, 8) == 250);

    printf("test_checksum_add passed\n");
}

void test_checksum_bits_to_int(void)
{
    assert(checksum_bits_to_int("000", 3) == 0);
    assert(checksum_bits_to_int("111", 3) == 7);
    assert(checksum_bits_to_int("101", 3) == 5);
    assert(checksum_bits_to_int("101010", 6) == 42);

    printf("test_checksum_bits_to_int passed\n");
}

void test_checksum_block_sum(void)
{
    // Data: "10110011", len=8, k=4
    // Blocks: "1011" (11) and "0011" (3)
    // 11 + 3 = 14 ("1110")
    int sum = checksum_block_sum("10110011", 8, 4);
    assert(sum == 14);

    // Data: "101", len=3, k=4 (padded with 0 to "1010" (10))
    sum = checksum_block_sum("101", 3, 4);
    assert(sum == 10);

    printf("test_checksum_block_sum passed\n");
}

void test_checksum_transmission(void)
{
    // Simulate transmission
    const char* data = "1011001110101111"; // 16 bits
    int len = 16;
    int k = 4;
    int mask = (1 << k) - 1;

    // Sender side
    int sum = checksum_block_sum(data, len, k);
    int checksum = (~sum) & mask;

    // We convert checksum to binary string
    char checksum_str[5];
    for (int i = 0; i < k; i++)
    {
        checksum_str[i] = ((checksum >> (k - 1 - i)) & 1) ? '1' : '0';
    }
    checksum_str[k] = '\0';

    // Receiver side: verification
    int checkword = checksum_bits_to_int(checksum_str, k);
    int rec_sum = checksum_block_sum(data, len, k);
    rec_sum = checksum_add(rec_sum, checkword, k);

    int complement = (~rec_sum) & mask;
    assert(complement == 0); // No error detected

    // Simulate error in data
    char corrupt_data[17];
    strcpy(corrupt_data, data);
    corrupt_data[0] = (corrupt_data[0] == '1') ? '0' : '1'; // Flip first bit

    int corrupt_rec_sum = checksum_block_sum(corrupt_data, len, k);
    corrupt_rec_sum = checksum_add(corrupt_rec_sum, checkword, k);
    int corrupt_complement = (~corrupt_rec_sum) & mask;
    assert(corrupt_complement != 0); // Error detected!

    printf("test_checksum_transmission passed\n");
}

int main(void)
{
    test_checksum_add();
    test_checksum_bits_to_int();
    test_checksum_block_sum();
    test_checksum_transmission();

    printf("All checksum tests passed successfully!\n");
    return 0;
}
