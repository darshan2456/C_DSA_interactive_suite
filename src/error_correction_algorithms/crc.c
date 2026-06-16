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

        int data_len = (int)strlen(data);
        int generator_len = (int)strlen(generator);

        if (generator_len < 2)
        {
            printf("\ngenerator polynomial must contain at least 2 bits.\n");
            continue;
        }

        char dividend[(CHECKSUM_MAX_BITS * 2) + 1];

        strcpy(dividend, data);

        for (int i = 0; i < generator_len - 1; i++)
        {
            dividend[data_len + i] = '0';
        }

        dividend[data_len + generator_len - 1] = '\0';

        printf("\nOriginal Data : %s", data);
        printf("\nGenerator     : %s", generator);
        printf("\nDividend      : %s\n", dividend);

        int dividend_len = (int)strlen(dividend);

        printf("\nPerforming modulo-2 division...\n");

        for (int i = 0; i <= dividend_len - generator_len; i++)
        {
            if (dividend[i] == '1')
            {
                crc_xor_operation(dividend, generator, i);
            }
        }

        char remainder[CHECKSUM_MAX_BITS + 1];
        for (int i = 0; i < generator_len - 1; i++)
        {
            remainder[i] = dividend[dividend_len - (generator_len - 1) + i];
        }
        remainder[generator_len - 1] = '\0';

        printf("\nCRC Remainder : %s", remainder);

        printf("\nTransmitted Codeword : %s%s\n", data, remainder);
    }
}