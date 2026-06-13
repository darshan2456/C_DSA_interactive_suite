#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>

/* New functions come here */

void error_correction_algorithms_demo(void)
{
    while (1)
    {
        int ECA_choice;

        /* Change the prompt and the range accordingly when new functions get added */
        int ECA_status = safe_input_int(&ECA_choice,
                                        "\nEnter 1 for Checksum (Sender)"
                                        "\nEnter 2 for Checksum (Receiver verification)"
                                        "\nEnter 3 for CRC (Sender)"
                                        "\nEnter 4 for CRC (Receiver verification)"
                                        "\nEnter 5 for LRC (Sender)"
                                        "\nEnter 6 for LRC (Receiver verification)"
                                        "\nEnter 7 for Parity Bit"
                                        "\nEnter 8 for VRC (Sender)"
                                        "\nEnter 9 for VRC (Receiver verification)"
                                        "\nEnter 10 for Hamming Code (Sender)"
                                        "\nEnter 11 for Hamming Code (Receiver verification)"
                                        "\nEnter -1 to exit: ",
                                        1, 11);

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
                lrc_receiver_demo();
                break;

            case 7:
                parity_bit_demo();
                break;

            case 8:
                vrc_demo();
                break;

            case 9:
                vrc_receiver_demo();
                break;

            case 10:
                hamming_demo();
                break;

            case 11:
                hamming_receiver_demo();
                break;

            default:
                printf("Wrong choice entered\n");
                break;
        }
    }
}