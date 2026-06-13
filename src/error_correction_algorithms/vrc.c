#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

void vrc_demo(void)
{
    while (1)
    {
        char data[CHECKSUM_MAX_BITS + 1];

        int data_status = safe_input_binary_string(data, sizeof(data),
                                                   "\n\nVRC Demo\n"
                                                   "enter binary data or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting VRC demo....");
            return;
        }

        if (data_status == 0)
        {
            continue;
        }

        int ones_count = 0;

        for (size_t i = 0; data[i] != '\0'; i++)
        {
            if (data[i] == '1')
            {
                ones_count++;
            }
        }

        int parity_bit = (ones_count % 2 == 0) ? 0 : 1;

        char transmitted_frame[CHECKSUM_MAX_BITS + 2];

        strcpy(transmitted_frame, data);

        size_t len = strlen(transmitted_frame);

        transmitted_frame[len] = parity_bit + '0';
        transmitted_frame[len + 1] = '\0';

        printf("\nData              : %s", data);
        printf("\nNumber of 1s      : %d", ones_count);
        printf("\nParity Type       : Even");
        printf("\nParity Bit        : %d", parity_bit);
        printf("\nTransmitted Frame : %s\n", transmitted_frame);
    }
}