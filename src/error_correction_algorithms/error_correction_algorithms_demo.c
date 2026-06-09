#include "safe_input.h"
#include <error_correction_algorithms.h>
#include <stdio.h>

/*New functions come here*/

void error_correction_algorithms_demo(void)
{
    while (1)
    {
        int ECA_choice;
        /*Change the prompt and the range accordingly when new functions get added*/
        int ECA_status = safe_input_int(
            &ECA_choice,
            "\nEnter 1 for checksum (sender)"
            "\nEnter 2 for checksum (receiver)"
            "\nEnter 3 for CRC"
            "\nEnter 4 for LRC"
            "\nEnter 5 for VRC"
            "\nEnter 6 for VRC Receiver"
            "\nEnter -1 to exit: ", 1, 6);

        if (ECA_status == INPUT_EXIT_SIGNAL)
        {
            printf("Exiting Error Correction Algorithm Demo....");
            return;
        }

        if (ECA_status == 0)
            continue;

        switch (ECA_choice)
        {

                /*Newly implemented functions will be called here*/

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
                lrc_demo();
                break;

            case 5:
                vrc_demo();
                break;

            case 6:
                vrc_receiver_demo();
                break;

            default:
                printf("Wrong choice entered");
                break;
        }
    }
}