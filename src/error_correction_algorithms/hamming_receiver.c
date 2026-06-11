#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

// hamming code receiver (decoder). given a received codeword we recompute the parity
// checks to form the syndrome: read as a binary number it is the 1-based position of a
// single-bit error (0 means no error). flipping that bit corrects the codeword.
void hamming_receiver_demo(void)
{
    while (1)
    {
        char frame[CHECKSUM_MAX_BITS + 1];

        int frame_status =
            safe_input_binary_string(frame, sizeof(frame),
                                 "\n\nHamming Code Receiver Demo\n"
                                 "enter received Hamming codeword or 'X' to exit:- ");

        if (frame_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Hamming Code Receiver demo....");
            return;
        }

        if (frame_status == 0)
        {
            continue;
        }

        int n = (int)strlen(frame);

        // ham[1..n] is the received codeword in transmission order (1-based, MSB end).
        int ham[CHECKSUM_MAX_BITS + 16];
        for (int pos = 1; pos <= n; pos++)
        {
            ham[pos] = frame[pos - 1] - '0';
        }

        // number of parity bits r = count of power-of-2 positions that fit in n.
        int r = 0;
        while ((1 << r) <= n)
        {
            r++;
        }

        // build the syndrome: bit i is the even-parity check over every position whose
        // index has bit i set (the parity bit itself is included in its own check).
        int syndrome = 0;
        for (int i = 0; i < r; i++)
        {
            int parity_pos = (1 << i);
            int parity = 0;

            for (int pos = 1; pos <= n; pos++)
            {
                if (pos & parity_pos)
                {
                    parity ^= ham[pos];
                }
            }

            if (parity)
            {
                syndrome += parity_pos;
            }
        }

        printf("\nReceived codeword    : %s", frame);
        printf("\nCodeword length (n)  : %d", n);
        printf("\nParity bits (r)      : %d", r);
        printf("\nSyndrome value       : %d", syndrome);

        if (syndrome == 0)
        {
            printf("\nResult               : no error detected -- data accepted");
        }
        else if (syndrome > n)
        {
            // the syndrome points past the frame: more than a single-bit error, which a
            // basic Hamming code cannot reliably locate.
            printf("\nResult               : syndrome %d is out of range -- "
                   "multiple-bit error, cannot correct",
                   syndrome);
        }
        else
        {
            printf("\nResult               : error detected at position %d -- correcting",
                   syndrome);
            ham[syndrome] ^= 1;

            printf("\nCorrected codeword   : ");
            for (int pos = 1; pos <= n; pos++)
            {
                printf("%d", ham[pos]);
            }
        }

        // recover the data bits (every position that is not a power of two).
        printf("\nRecovered data bits  : ");
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & (pos - 1)) != 0)
            {
                printf("%d", ham[pos]);
            }
        }
        printf("\n");
    }
}
