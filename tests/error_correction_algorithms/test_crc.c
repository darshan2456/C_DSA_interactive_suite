#include "error_correction_algorithms.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_crc_generate_and_verify(void)
{
    // Test data and generator
    const char* data = "1101011011";
    const char* generator = "10011"; // CRC-4
    char remainder[CHECKSUM_MAX_BITS + 1];
    char codeword[CHECKSUM_MAX_BITS * 2 + 1];

    crc_generate(data, generator, remainder, codeword);

    // Verify remainder size: length of generator - 1
    assert(strlen(remainder) == strlen(generator) - 1);
    assert(strcmp(remainder, "1110") == 0); // Modulo-2 remainder for this input

    // Codeword should be data + remainder
    char expected_codeword[128];
    snprintf(expected_codeword, sizeof(expected_codeword), "%s%s", data, remainder);
    assert(strcmp(codeword, expected_codeword) == 0);

    // Verify the clean codeword
    int check_valid = crc_verify(codeword, generator, NULL);
    assert(check_valid == 1); // Should be clean

    // Simulate bit flip error in data
    char corrupt_codeword[128];
    strcpy(corrupt_codeword, codeword);
    corrupt_codeword[0] = (corrupt_codeword[0] == '1') ? '0' : '1';

    int check_corrupt = crc_verify(corrupt_codeword, generator, NULL);
    assert(check_corrupt == 0); // Should detect error

    // Simulate bit flip in parity remainder
    strcpy(corrupt_codeword, codeword);
    int last_idx = (int)strlen(corrupt_codeword) - 1;
    corrupt_codeword[last_idx] = (corrupt_codeword[last_idx] == '1') ? '0' : '1';

    check_corrupt = crc_verify(corrupt_codeword, generator, NULL);
    assert(check_corrupt == 0); // Should detect error

    printf("test_crc_generate_and_verify passed\n");
}

void test_crc_xor_operation(void)
{
    char div[32];
    strcpy(div, "110100");
    crc_xor_operation(div, "1011", 0);
    assert(strncmp(div, "0110", 4) == 0); // 1101 XOR 1011 = 0110

    printf("test_crc_xor_operation passed\n");
}

int main(void)
{
    test_crc_xor_operation();
    test_crc_generate_and_verify();

    printf("All CRC tests passed successfully!\n");
    return 0;
}
