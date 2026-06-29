#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

#define LRC_MAX_ROWS 20
#define LRC_MAX_COLS 64

void lrc_receiver_demo(void)
{
    int rows;

    printf("\n=== LRC Receiver Verification ===\n");

    int result =
        safe_input_int(&rows, "Enter number of received data words (1-20): ", 1, LRC_MAX_ROWS);

    if (result == -111)
    {
        printf("Exiting LRC receiver demo...\n");
        return;
    }

    if (result == 0)
    {
        printf("Invalid input. Returning to main menu.\n");
        return;
    }

    char data[LRC_MAX_ROWS][LRC_MAX_COLS + 1];
    int cols = -1;

    printf("Enter received data words:\n");

    for (int i = 0; i < rows; i++)
    {
        char prompt_buf[32];
        snprintf(prompt_buf, sizeof(prompt_buf), "  Word %d: ", i + 1);
        int status_w = safe_input_string(data[i], sizeof(data[i]), prompt_buf);
        if (status_w == INPUT_EXIT_SIGNAL)
        {
            printf("Exiting LRC receiver demo...\n");
            return;
        }

        int len = (int)strlen(data[i]);

        if (len == 0)
        {
            printf("Error: word cannot be empty.\n");
            return;
        }

        for (int j = 0; j < len; j++)
        {
            if (data[i][j] != '0' && data[i][j] != '1')
            {
                printf("Error: word must contain only 0s and 1s.\n");
                return;
            }
        }

        if (cols == -1)
        {
            cols = len;
        }
        else if (len != cols)
        {
            printf("Error: all data words must have the same length.\n");
            return;
        }
    }

    char received_lrc[LRC_MAX_COLS + 1];

    int status_lrc = safe_input_string(received_lrc, sizeof(received_lrc), "Enter received LRC: ");
    if (status_lrc == INPUT_EXIT_SIGNAL)
    {
        printf("Exiting LRC receiver demo...\n");
        return;
    }

    int lrc_len = (int)strlen(received_lrc);

    if (lrc_len != cols)
    {
        printf("Error: LRC length must match data word length.\n");
        return;
    }

    char computed_lrc[LRC_MAX_COLS + 1];

    for (int j = 0; j < cols; j++)
    {
        int ones = 0;

        for (int i = 0; i < rows; i++)
        {
            if (data[i][j] == '1')
            {
                ones++;
            }
        }

        computed_lrc[j] = (ones % 2 == 0) ? '0' : '1';
    }

    computed_lrc[cols] = '\0';

    printf("\nReceived LRC : %s\n", received_lrc);

    printf("Computed LRC : %s\n", computed_lrc);

    if (strcmp(received_lrc, computed_lrc) == 0)
    {
        printf("\n[SUCCESS] No error detected. Data accepted.\n");
    }
    else
    {
        printf("\n[ERROR] Transmission error detected.\n");
    }
}