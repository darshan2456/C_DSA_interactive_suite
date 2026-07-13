#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Helper to validate if a string is a non-empty binary string */
static bool validate_binary_string(const char* data)
{
    if (!data || strlen(data) == 0)
    {
        return false;
    }

    for (size_t i = 0; i < strlen(data); i++)
    {
        if (data[i] != '0' && data[i] != '1')
        {
            return false;
        }
    }

    return true;
}

/* Helper to count number of 1s */
static int count_ones(const char* data)
{
    int count = 0;

    for (size_t i = 0; i < strlen(data); i++)
    {
        if (data[i] == '1')
        {
            count++;
        }
    }

    return count;
}

/* Even parity generator */
int generateEvenParity(const char* data)
{
    if (!validate_binary_string(data))
    {
        return -1;
    }

    int ones = count_ones(data);
    return (ones % 2 == 0) ? 0 : 1;
}

/* Odd parity generator */
int generateOddParity(const char* data)
{
    if (!validate_binary_string(data))
    {
        return -1;
    }

    int ones = count_ones(data);
    return (ones % 2 == 0) ? 1 : 0;
}

/* Even parity verification */
int verifyEvenParity(const char* receivedData)
{
    if (!validate_binary_string(receivedData))
    {
        return -1;
    }

    int ones = count_ones(receivedData);
    return (ones % 2 == 0) ? 1 : 0;
}

/* Odd parity verification */
int verifyOddParity(const char* receivedData)
{
    if (!validate_binary_string(receivedData))
    {
        return -1;
    }

    int ones = count_ones(receivedData);
    return (ones % 2 == 1) ? 1 : 0;
}

/* ================= MAIN DEMO ================= */

void parity_bit_demo(void)
{
    char data[CHECKSUM_MAX_BITS + 1];
    char received[CHECKSUM_MAX_BITS + 2];
    int choice;

    printf("\n--- Parity Bit Error Detection Demo ---\n");

    /* INPUT DATA */
    while (1)
    {
        int data_status =
            safe_input_binary_string(data, sizeof(data), "\nEnter binary data or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Parity Bit demo...\n");
            return;
        }

        if (data_status == 0)
        {
            continue;
        }

        break;
    }

    /* PARITY CHOICE */
    while (1)
    {
        int status = safe_input_int(&choice,
                                    "\nSelect Parity Mode:\n"
                                    "1. Even Parity\n"
                                    "2. Odd Parity\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 2);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Parity Bit demo...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        break;
    }

    int parity_bit = 0;

    if (choice == 1)
    {
        parity_bit = generateEvenParity(data);
        printf("\nMode        : Even Parity\n");
    }
    else
    {
        parity_bit = generateOddParity(data);
        printf("\nMode        : Odd Parity\n");
    }

    printf("Original Data: %s (Length: %zu, 1s count: %d)\n", data, strlen(data), count_ones(data));

    printf("Parity Bit   : %d\n", parity_bit);
    printf("Transmitted  : %s%d\n", data, parity_bit);

    /* RECEIVER INPUT */
    while (1)
    {
        int recv_status = safe_input_binary_string(
            received, sizeof(received), "\nEnter received binary string or 'X' to exit:- ");

        if (recv_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Parity Bit demo...\n");
            return;
        }

        if (recv_status == 0)
        {
            continue;
        }

        break;
    }

    /* VERIFICATION */
    int result = 0;

    if (choice == 1)
    {
        result = verifyEvenParity(received);
    }
    else
    {
        result = verifyOddParity(received);
    }

    if (result == 1)
    {
        printf("\n[SUCCESS] No single-bit errors detected.\n");
    }
    else if (result == 0)
    {
        printf("\n[ERROR] Parity mismatch! Transmission error detected.\n");
    }
    else
    {
        printf("\n[ERROR] Invalid input format.\n");
    }
}