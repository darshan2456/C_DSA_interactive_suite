#include "error_correction_algorithms.h"
#include "history_logger.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// converts a k-bit binary string into its integer value
int checksum_bits_to_int(const char* bits, int k)
{
    int value = 0;
    for (int i = 0; i < k; i++)
    {
        value = (value << 1) | (bits[i] - '0');
    }
    return value;
}
// checksum (receiver side): re-runs the one's-complement block sum over the received
// data, then folds in the received checksum block. if the one's complement of the
// total is all zeros the frame is accepted, otherwise an error is detected. this is
// the verification counterpart to the sender in checksum.c.
void checksum_receiver_demo(void)
{
    while (1)
    {
        int k;
        int k_status = safe_input_int(
            &k,
            "\n\nchecksum (receiver side)\nenter the block size k in bits (1 to 16), "
            "or -1 to exit:- ",
            1, 16);

        if (k_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting checksum receiver demo....");
            return;
        }
        if (k_status == 0)
        {
            continue;
        }

        char data[CHECKSUM_MAX_BITS + 1];
        int data_status = safe_input_binary_string(
            data, sizeof(data),
            "enter the received binary data (digits 0/1 only), or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting checksum receiver demo....");
            return;
        }
        if (data_status == 0)
        {
            continue;
        }

        char check[CHECKSUM_MAX_BITS + 1];
        int check_status = safe_input_binary_string(
            check, sizeof(check),
            "enter the received checksum (exactly k bits), or 'X' to exit:- ");

        if (check_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting checksum receiver demo....");
            return;
        }
        if (check_status == 0)
        {
            continue;
        }

        if ((int)strlen(check) != k)
        {
            printf("\ninvalid: the checksum must be exactly %d bit(s) long.\n", k);
            continue;
        }

        int len = (int)strlen(data);
        int mask = (1 << k) - 1;

        // sum the data blocks (same as the sender), then fold in the received checksum
        int sum = checksum_block_sum(data, len, k);
        clock_t start_t = clock();
        int checkword = checksum_bits_to_int(check, k);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        add_to_history("Checksum Receiver", k, total_t);
        sum = checksum_add(sum, checkword, k);

        printf("checksum: ");
        checksum_print_binary(checkword, k);
        printf("  (value %d)  running sum = ", checkword);
        checksum_print_binary(sum, k);
        printf("\n");

        int complement = (~sum) & mask;

        printf("\nfinal sum  = ");
        checksum_print_binary(sum, k);
        printf("\ncomplement = ");
        checksum_print_binary(complement, k);

        // a clean frame sums to all ones, so its complement is all zeros
        if (complement == 0)
        {
            printf("\n\nresult: no error detected -- data accepted.\n");
        }
        else
        {
            printf("\n\nresult: error detected -- data rejected.\n");
        }
    }
}