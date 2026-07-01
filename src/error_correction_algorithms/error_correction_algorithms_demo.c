#include "display_header.h"
#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>

/* New functions come here */

void error_correction_algorithms_demo(void)
{
    while (1)
    {
        display_header("Error Correction Algorithms");

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
                display_header("Checksum (Sender)");
                checksum_demo();
                break;

            case 2:
                display_header("Checksum (Receiver)");
                checksum_receiver_demo();
                break;

            case 3:
                display_header("CRC (Sender)");
                crc_demo();
                break;

            case 4:
                display_header("CRC (Receiver)");
                crc_receiver_demo();
                break;

            case 5:
                display_header("LRC (Sender)");
                lrc_demo();
                break;

            case 6:
                display_header("LRC (Receiver)");
                lrc_receiver_demo();
                break;

            case 7:
                display_header("Parity Bit");
                parity_bit_demo();
                break;

            case 8:
                display_header("VRC (Sender)");
                vrc_demo();
                break;

            case 9:
                display_header("VRC (Receiver)");
                vrc_receiver_demo();
                break;

            case 10:
                display_header("Hamming Code (Sender)");
                hamming_demo();
                break;

            case 11:
                display_header("Hamming Code (Receiver)");
                hamming_receiver_demo();
                break;

            default:
                printf("Wrong choice entered\n");
                break;
        }
    }
}