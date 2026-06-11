#include "error_correction_algorithms.h"
#include "history_logger.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// prints the low `bits` bits of value, most-significant bit first. shared helper
// used for displaying k-bit words/checksums in the sender and receiver demos.
void checksum_print_binary(int value, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}

// reads a line of binary data ('0'/'1' only) into buff. mirrors the string-input
// convention used by the expression module (validate_infix_expr): returns 1 on a
// valid non-empty binary string, 0 on EOF / invalid input (caller should re-prompt),
// and INPUT_EXIT_SIGNAL when the user types 'X' to leave the demo.
int safe_input_binary_string(char* buff, size_t size, const char* prompt)
{
    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (!fgets(buff, size, stdin))
    { // EOF or read error
        clearerr(stdin);
        printf("\ninput ended unexpectedly");
        return 0;
    }
    buff[strcspn(buff, "\n")] = '\0';

    if (buff[0] == 'X' && buff[1] == '\0')
    { // user asked to leave the demo
        return INPUT_EXIT_SIGNAL;
    }
    if (buff[0] == '\0')
    {
        printf("\ninvalid: empty input. enter a binary string like 1101001111\n");
        return 0;
    }

    for (size_t i = 0; buff[i] != '\0'; i++)
    {
        if (buff[i] != '0' && buff[i] != '1')
        {
            printf("\ninvalid character: %c\nonly binary digits 0 and 1 are allowed.\n", buff[i]);
            return 0;
        }
    }
    return 1;
}

// one's-complement (end-around carry) addition of a single k-bit word into the
// running sum: any overflow above k bits is wrapped back into the low k bits,
// e.g. for k=3: 111 + 001 = 1000 -> 000 + 1 = 001.
int checksum_add(int sum, int word, int k)
{
    int mask = (1 << k) - 1;
    sum += word;
    while (sum > mask)
    {
        sum = (sum & mask) + (sum >> k);
    }
    return sum;
}

// splits the `len`-bit binary string `data` into k-bit blocks (padding the final
// block with trailing 0s) and accumulates them with one's-complement addition,
// printing each block and the running sum. returns the final k-bit folded sum.
int checksum_block_sum(const char* data, int len, int k)
{
    // pad the length up to a whole number of k-bit blocks; the missing low-order
    // bits of the last block are treated as 0 (we never read past `len`).
    int padded_len = len;
    while (padded_len % k != 0)
    {
        padded_len++;
    }

    printf("\ndata length = %d bit(s), block size k = %d, padded to %d bit(s) (%d block(s))\n", len,
           k, padded_len, padded_len / k);

    int sum = 0;
    for (int i = 0; i < padded_len; i += k)
    {
        int word = 0;

        // assemble the current block, padding with 0 once we pass the real data
        for (int j = 0; j < k; j++)
        {
            word <<= 1;
            if (i + j < len)
            {
                word |= (data[i + j] - '0');
            }
        }

        sum = checksum_add(sum, word, k);

        printf("block %d: ", (i / k) + 1);
        checksum_print_binary(word, k);
        printf("  (value %d)  running sum = ", word);
        checksum_print_binary(sum, k);
        printf("\n");
    }
    return sum;
}

// checksum (sender side): the data is split into k-bit blocks and summed using
// one's-complement (end-around carry) addition; the checksum is the one's complement
// of that running sum. this is the value the sender appends to the data before
// transmitting it. the receiver-side verification lives in checksum_receiver.c.
void checksum_demo(void)
{
    while (1)
    {
        int k;
        int k_status =
            safe_input_int(&k,
                           "\n\nchecksum (sender side)\nenter the block size k in bits (1 to 16), "
                           "or -1 to exit:- ",
                           1, 16);

        if (k_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting checksum demo....");
            return;
        }
        if (k_status == 0)
        {
            continue;
        }

        char data[CHECKSUM_MAX_BITS + 1];
        int data_status = safe_input_binary_string(
            data, sizeof(data), "enter the binary data (digits 0/1 only), or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting checksum demo....");
            return;
        }
        if (data_status == 0)
        {
            continue;
        }

        int len = (int)strlen(data);
        int mask = (1 << k) - 1; // keeps only the low k bits

        clock_t start_t = clock();
        int sum = checksum_block_sum(data, len, k);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        add_to_history("Checksum Sender", len, total_t);
        int checksum = (~sum) & mask; // one's complement of the final sum, kept to k bits

        printf("\nfinal sum       = ");
        checksum_print_binary(sum, k);
        printf("\nchecksum (~sum) = ");
        checksum_print_binary(checksum, k);
        printf("\n\nthe sender appends this checksum to the data before transmission.\n");
    }
}
