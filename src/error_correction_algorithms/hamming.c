#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

// hamming code generation (sender side). the user supplies the binary data bits and
// we build the (n, k) codeword by inserting r even-parity bits at the power-of-2
// positions (1, 2, 4, 8, ...). a 1-based layout keeps the parity maths readable.
void hamming_demo(void)
{
    while (1)
    {
        char data[CHECKSUM_MAX_BITS + 1];

        int data_status = safe_input_binary_string(data, sizeof(data),
                                               "\n\nHamming Code Demo\n"
                                               "enter binary data bits or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Hamming Code demo....");
            return;
        }

        if (data_status == 0)
        {
            continue;
        }

        int k = (int)strlen(data);

        // number of parity bits r: the smallest r with 2^r >= k + r + 1, so that every
        // codeword position (data + parity) can be addressed by the r parity checks.
        int r = 0;
        while ((1 << r) < (k + r + 1))
        {
            r++;
        }

        int n = k + r;

        // ham[1..n] is the codeword in transmission order (position 1 is the MSB end).
        int ham[CHECKSUM_MAX_BITS + 16];

        // lay out the frame: power-of-2 positions are parity placeholders (filled below),
        // every other position takes the next data bit in order.
        int data_index = 0;
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & (pos - 1)) == 0) // pos is a power of two -> parity position
            {
                ham[pos] = 0;
            }
            else
            {
                ham[pos] = data[data_index] - '0';
                data_index++;
            }
        }

        // compute each parity bit (even parity) over the positions it covers: parity bit
        // at position p = 2^i covers every position whose index has bit i set.
        for (int i = 0; i < r; i++)
        {
            int parity_pos = (1 << i);
            int parity = 0;

            for (int pos = 1; pos <= n; pos++)
            {
                if ((pos & parity_pos) && pos != parity_pos)
                {
                    parity ^= ham[pos];
                }
            }

            ham[parity_pos] = parity;
        }

        printf("\nData bits (k)        : %d", k);
        printf("\nParity bits (r)      : %d", r);
        printf("\nCodeword length (n)  : %d", n);

        printf("\nParity bit values    :");
        for (int i = 0; i < r; i++)
        {
            int parity_pos = (1 << i);
            printf(" P%d=%d", parity_pos, ham[parity_pos]);
        }

        printf("\nHamming Code (P=parity, transmitted left to right):\n");
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & (pos - 1)) == 0)
            {
                printf("P%d=%d ", pos, ham[pos]);
            }
            else
            {
                printf("D%d=%d ", pos, ham[pos]);
            }
        }

        printf("\nGenerated Hamming Code (%d,%d): ", n, k);
        for (int pos = 1; pos <= n; pos++)
        {
            printf("%d", ham[pos]);
        }
        printf("\n");
    }
}
