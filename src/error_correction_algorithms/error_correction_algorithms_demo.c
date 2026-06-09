#include "safe_input.h"
#include "error_correction_algorithms.h"
#include <stdio.h>

/* New functions come here */

void error_correction_algorithms_demo(void)
{
    while (1)
    {
        int ECA_choice;

        /* Change the prompt and the range accordingly when new functions get added */
        int ECA_status = safe_input_int(
            &ECA_choice,
            "\nEnter 1 for Checksum (Sender)"
            "\nEnter 2 for Checksum (Receiver verification)"
            "\nEnter 3 for CRC (Sender)"
            "\nEnter 4 for CRC (Receiver verification)"
            "\nEnter 5 for LRC"
            "\nEnter 6 for Parity Bit"
            "\nEnter 7 for VRC"
            "\nEnter 8 for Hamming Code"
            "\nEnter -1 to exit: ",
            1, 8);

        if (ECA_status == INPUT_EXIT_SIGNAL)
        {
            printf("Exiting Error Correction Algorithm Demo...\n");
            return;
        }

        if (ECA_status == 0)
            continue;

        switch (ECA_choice)
        {
            case 1:
                checksum_demo();
                break;

            case 2:
                checksum_receiver_demo();
                break;

            case 3:
                crc_demo();
                break;

            case 4:
                crc_receiver_demo();
                break;

            case 5:
                lrc_demo();
                break;

            case 6:
                parity_bit_demo();
                break;

            case 7:
                vrc_demo();
                break;

            case 8:
                hamming_demo();
                break;

            default:
                printf("Wrong choice entered\n");
                break;
        }
    }
}