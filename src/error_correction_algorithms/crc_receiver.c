#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

void crc_receiver_demo(void)
{
    while (1)
    {
        char received_codeword[(CHECKSUM_MAX_BITS * 2) + 1];

        int codeword_status = safe_input_binary_string(
            received_codeword, sizeof(received_codeword),
            "\n\nCRC Receiver Verification\n"
            "Enter transmitted codeword (data + CRC bits) or 'X' to exit:- ");

        if (codeword_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting CRC receiver demo....");
            return;
        }

        if (codeword_status == 0)
        {
            continue;
        }

        char generator[CHECKSUM_MAX_BITS + 1];

        int generator_status = safe_input_binary_string(
            generator, sizeof(generator), "Enter generator polynomial or 'X' to exit:- ");

        if (generator_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting CRC receiver demo....");
            return;
        }

        if (generator_status == 0)
        {
            continue;
        }

        int generator_len = (int)strlen(generator);

        if (generator_len < 2)
        {
            printf("\nGenerator polynomial must contain at least 2 bits.\n");
            continue;
        }

        int codeword_len = (int)strlen(received_codeword);
        if (codeword_len < generator_len)
        {
            printf("\nError: codeword length (%d) must be at least as long as the generator "
                   "polynomial (%d).\n",
                   codeword_len, generator_len);
            continue;
        }

        char remainder[CHECKSUM_MAX_BITS + 1];
        int is_valid = crc_verify(received_codeword, generator, remainder);

        printf("\nComputed Remainder : %s\n", remainder);

        if (!is_valid)
        {
            printf("\n[ERROR] Transmission error detected.\n");
        }
        else
        {
            printf("\n[SUCCESS] No error detected. Data accepted.\n");
        }
    }
}