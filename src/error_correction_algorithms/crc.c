#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

void crc_xor_operation(char* dividend, const char* divisor, int pos)
{
    int n = (int)strlen(divisor);

    for (int i = 0; i < n; i++)
    {
        dividend[pos + i] = (dividend[pos + i] == divisor[i]) ? '0' : '1';
    }
}

// crc_generate: computes the CRC remainder for `data` using `generator` via
// modulo-2 (XOR) division.  The remainder is written to `remainder_out` and,
// if `codeword_out` is not NULL, the full transmitted codeword (data + CRC) is
// written there too.
void crc_generate(const char* data, const char* generator, char* remainder_out, char* codeword_out)
{
    int data_len = (int)strlen(data);
    int generator_len = (int)strlen(generator);

    // Append (generator_len - 1) zero bits to data to form the dividend
    char dividend[(CHECKSUM_MAX_BITS * 2) + 1];
    strcpy(dividend, data);
    for (int i = 0; i < generator_len - 1; i++)
    {
        dividend[data_len + i] = '0';
    }
    int dividend_len = data_len + generator_len - 1;
    dividend[dividend_len] = '\0';

    // Modulo-2 division
    for (int i = 0; i <= dividend_len - generator_len; i++)
    {
        if (dividend[i] == '1')
        {
            crc_xor_operation(dividend, generator, i);
        }
    }

    // Extract the remainder (last generator_len-1 bits)
    for (int i = 0; i < generator_len - 1; i++)
    {
        remainder_out[i] = dividend[dividend_len - (generator_len - 1) + i];
    }
    remainder_out[generator_len - 1] = '\0';

    if (codeword_out != NULL)
    {
        strcpy(codeword_out, data);
        strcat(codeword_out, remainder_out);
    }
}

// crc_verify: re-divides a received `codeword` by `generator`.  If the
// remainder is all zeros the frame is clean and 1 is returned; otherwise 0 is
// returned (error detected).  The computed remainder is optionally written to
// `remainder_out`.
int crc_verify(const char* codeword, const char* generator, char* remainder_out)
{
    int generator_len = (int)strlen(generator);
    int codeword_len = (int)strlen(codeword);

    char dividend[(CHECKSUM_MAX_BITS * 2) + 1];
    strcpy(dividend, codeword);

    for (int i = 0; i <= codeword_len - generator_len; i++)
    {
        if (dividend[i] == '1')
        {
            crc_xor_operation(dividend, generator, i);
        }
    }

    // The remainder is the last (generator_len-1) bits
    char remainder[CHECKSUM_MAX_BITS + 1];
    strcpy(remainder, &dividend[codeword_len - (generator_len - 1)]);

    if (remainder_out != NULL)
    {
        strcpy(remainder_out, remainder);
    }

    for (int i = 0; remainder[i] != '\0'; i++)
    {
        if (remainder[i] == '1')
        {
            return 0; // Error detected
        }
    }
    return 1; // No error
}

void crc_demo(void)
{
    while (1)
    {
        char data[CHECKSUM_MAX_BITS + 1];
        int data_status = safe_input_binary_string(
            data, sizeof(data), "\n\nCRC Demo\nEnter binary data or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting CRC demo....");
            return;
        }

        if (data_status == 0)
        {
            continue;
        }

        char generator[CHECKSUM_MAX_BITS + 1];
        int generator_status = safe_input_binary_string(
            generator, sizeof(generator), "Enter generator polynomial or 'X' to exit:- ");

        if (generator_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting CRC demo....");
            return;
        }

        if (generator_status == 0)
        {
            continue;
        }

        int generator_len = (int)strlen(generator);

        if (generator_len < 2)
        {
            printf("\ngenerator polynomial must contain at least 2 bits.\n");
            continue;
        }

        char remainder[CHECKSUM_MAX_BITS + 1];
        char codeword[(CHECKSUM_MAX_BITS * 2) + 1];

        crc_generate(data, generator, remainder, codeword);

        printf("\nOriginal Data : %s", data);
        printf("\nGenerator     : %s", generator);
        printf("\nCRC Remainder : %s", remainder);
        printf("\nTransmitted Codeword : %s\n", codeword);
    }
}