#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

void vrc_receiver_demo(void)
{
    while (1)
    {
        char received_frame[CHECKSUM_MAX_BITS + 2];

        int frame_status = safe_input_binary_string(received_frame, sizeof(received_frame),
                                                    "\n\nVRC Receiver Verification\n"
                                                    "Enter received frame or 'X' to exit:- ");

        if (frame_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting VRC receiver demo....");
            return;
        }

        if (frame_status == 0)
        {
            continue;
        }

        int ones_count = 0;

        for (size_t i = 0; received_frame[i] != '\0'; i++)
        {
            if (received_frame[i] == '1')
            {
                ones_count++;
            }
        }

        printf("\nReceived Frame : %s", received_frame);
        printf("\nNumber of 1s   : %d", ones_count);
        printf("\nParity Type    : Even");

        if (ones_count % 2 == 0)
        {
            printf("\n\n[SUCCESS] No error detected.\n");
        }
        else
        {
            printf("\n\n[ERROR] Transmission error detected.\n");
        }
    }
}