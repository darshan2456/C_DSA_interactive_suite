#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Helper to validate if a string is a non-empty binary string */
static bool validate_binary_string(const char *data) {
    if (!data || strlen(data) == 0) {
        return false;
    }
    for (size_t i = 0; i < strlen(data); i++) {
        if (data[i] != '0' && data[i] != '1') {
            return false;
        }
    }
    return true;
}

/* Helper to count the number of 1s in a binary string */
static int count_ones(const char *data) {
    int count = 0;
    for (size_t i = 0; i < strlen(data); i++) {
        if (data[i] == '1') {
            count++;
        }
    }
    return count;
}

/* Generate even parity bit for a binary string. Returns -1 on invalid input. */
int generateEvenParity(const char *data) {
    if (!validate_binary_string(data)) {
        return -1;
    }
    int ones = count_ones(data);
    return (ones % 2 == 0) ? 0 : 1;
}

/* Generate odd parity bit for a binary string. Returns -1 on invalid input. */
int generateOddParity(const char *data) {
    if (!validate_binary_string(data)) {
        return -1;
    }
    int ones = count_ones(data);
    return (ones % 2 == 0) ? 1 : 0;
}

/* Verify received data using even parity. Returns 1 if valid, 0 if error, -1 on invalid input. */
int verifyEvenParity(const char *receivedData) {
    if (!validate_binary_string(receivedData)) {
        return -1;
    }
    int ones = count_ones(receivedData);
    return (ones % 2 == 0) ? 1 : 0;
}

/* Verify received data using odd parity. Returns 1 if valid, 0 if error, -1 on invalid input. */
int verifyOddParity(const char *receivedData) {
    if (!validate_binary_string(receivedData)) {
        return -1;
    }
    int ones = count_ones(receivedData);
    return (ones % 2 == 1) ? 1 : 0;
}

/* Interactive CLI Demo for Parity Bit Error Detection */
void parity_bit_demo(void) {
    char data[256];
    char received[256];
    int choice;

    printf("\n--- Parity Bit Error Detection Demo ---\n");

    while (1) {
        printf("\nEnter a binary data string (maximum 255 chars, '0' or '1' only), or '-1' to exit: ");
        if (scanf("%255s", data) != 1) {
            continue;
        }

        if (strcmp(data, "-1") == 0) {
            printf("Exiting Parity Bit demo...\n");
            return;
        }

        if (!validate_binary_string(data)) {
            printf("Invalid input! Please enter binary characters ('0' or '1') only.\n");
            continue;
        }
        break;
    }

    while (1) {
        int status = safe_input_int(&choice, 
            "\nSelect Parity Mode:\n"
            "1. Even Parity\n"
            "2. Odd Parity\n"
            "Enter choice: ", 1, 2);

        if (status == INPUT_EXIT_SIGNAL) {
            printf("Exiting Parity Bit demo...\n");
            return;
        }
        if (status == 0) continue;
        break;
    }

    int parity_bit = 0;
    if (choice == 1) {
        parity_bit = generateEvenParity(data);
        printf("\nMode        : Even Parity\n");
    } else {
        parity_bit = generateOddParity(data);
        printf("\nMode        : Odd Parity\n");
    }

    printf("Original Data: %s (Length: %zu, 1s count: %d)\n", data, strlen(data), count_ones(data));
    printf("Parity Bit   : %d\n", parity_bit);
    printf("Transmitted  : %s%d\n", data, parity_bit);

    while (1) {
        printf("\nEnter the received binary string to verify (or change a bit of '%s%d' to simulate an error):\nReceived: ", data, parity_bit);
        if (scanf("%255s", received) != 1) {
            continue;
        }

        if (!validate_binary_string(received)) {
            printf("Invalid binary string! Try again.\n");
            continue;
        }
        break;
    }

    int result = 0;
    if (choice == 1) {
        result = verifyEvenParity(received);
    } else {
        result = verifyOddParity(received);
    }

    if (result == 1) {
        printf("\n✅ Verification Successful: No single-bit errors detected.\n");
    } else if (result == 0) {
        printf("\n❌ Verification Failed: Parity mismatch! Transmission error detected.\n");
    } else {
        printf("\n⚠️ Verification Error: Invalid input format.\n");
    }
}
