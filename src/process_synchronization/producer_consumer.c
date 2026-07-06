#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "process_synchronization.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 5

static void display_buffer_state(const int* buffer, int in, int out, int mutex, int empty, int full,
                                 int prod_blocked, int cons_blocked)
{
    printf("\n\033[1;34m┌────────────────────────────────────────────────────────┐\033[0m");
    printf("\n\033[1;34m│                 PRODUCER-CONSUMER STATE                │\033[0m");
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m");
    printf("\n│ Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (buffer[i] == 0)
        {
            printf("  -  ");
        }
        else
        {
            printf(" P%-3d", buffer[i]);
        }
        if (i < BUFFER_SIZE - 1)
        {
            printf(" |");
        }
    }
    printf(" ]           │");
    printf("\n│           in = %d, out = %d                              │", in, out);
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m");
    printf("\n│ Semaphores:                                            │");
    printf("\n│   mutex     = %-10d (Mutual exclusion lock)       │", mutex);
    printf("\n│   empty_sem = %-10d (Available empty slots)       │", empty);
    printf("\n│   full_sem  = %-10d (Filled item slots)           │", full);
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m");
    printf("\n│ Process States:                                        │");
    printf("\n│   Producer : %-50s   │", prod_blocked ? "\033[1;31mBLOCKED (Buffer Full)\033[0m"
                                                      : "\033[1;32mACTIVE/READY\033[0m");
    printf("\n│   Consumer : %-50s   │", cons_blocked ? "\033[1;31mBLOCKED (Buffer Empty)\033[0m"
                                                      : "\033[1;32mACTIVE/READY\033[0m");
    printf("\n\033[1;34m└────────────────────────────────────────────────────────┘\033[0m\n");
}

void producer_consumer_demo(void)
{
    int buffer[BUFFER_SIZE] = {0};
    int in = 0;
    int out = 0;
    int mutex = 1;
    int empty = BUFFER_SIZE;
    int full = 0;
    int item_counter = 1;
    int prod_blocked = 0;
    int cons_blocked = 0;

    while (1)
    {
        if (!is_instant())
        {
            clear_screen();
        }
        printf("\n\033[1;36m=== PRODUCER-CONSUMER PROBLEM SIMULATOR ===\033[0m\n");
        printf("This demo visualizes the behavior of Producer and Consumer processes\n");
        printf("synchronizing over a bounded circular buffer using Semaphores.\n");

        display_buffer_state(buffer, in, out, mutex, empty, full, prod_blocked, cons_blocked);

        printf("\nOptions:\n");
        printf("1. Run Producer (attempt to produce)\n");
        printf("2. Run Consumer (attempt to consume)\n");
        printf("3. Auto-simulate (runs random steps automatically)\n");
        printf("4. Reset Buffer state\n");
        printf("Enter option (-1 to return to menu): ");
        fflush(stdout);

        int choice;
        int status = safe_input_int(&choice, "", 1, 4);
        if (status == INPUT_EXIT_SIGNAL || choice == -1)
        {
            break;
        }
        if (status == 0)
        {
            continue;
        }

        if (choice == 1)
        {
            printf("\n\033[1;33m[Action: Producer Attempting to Produce]\033[0m\n");
            // wait(empty)
            if (empty == 0)
            {
                prod_blocked = 1;
                printf("\033[1;31mProducer: Empty Semaphore is 0 (Buffer full). Producer is "
                       "BLOCKED.\033[0m\n");
            }
            else
            {
                // wait(mutex)
                if (mutex == 0)
                {
                    printf("Producer: Mutex is held by another process. Waiting.\n");
                }
                else
                {
                    prod_blocked = 0;
                    empty--;
                    mutex--; // acquire lock

                    buffer[in] = item_counter++;
                    int produced = buffer[in];
                    printf(
                        "\033[1;32mProducer: Acquired mutex, placed item P%d at slot %d.\033[0m\n",
                        produced, in);
                    in = (in + 1) % BUFFER_SIZE;

                    mutex++; // release lock
                    full++;

                    if (cons_blocked)
                    {
                        cons_blocked = 0;
                        printf("\033[1;32mProducer: Signal(full_sem) sent. Consumer is now "
                               "UNBLOCKED.\033[0m\n");
                    }
                }
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 2)
        {
            printf("\n\033[1;33m[Action: Consumer Attempting to Consume]\033[0m\n");
            // wait(full)
            if (full == 0)
            {
                cons_blocked = 1;
                printf("\033[1;31mConsumer: Full Semaphore is 0 (Buffer empty). Consumer is "
                       "BLOCKED.\033[0m\n");
            }
            else
            {
                // wait(mutex)
                if (mutex == 0)
                {
                    printf("Consumer: Mutex is held by another process. Waiting.\n");
                }
                else
                {
                    cons_blocked = 0;
                    full--;
                    mutex--; // acquire lock

                    int consumed = buffer[out];
                    buffer[out] = 0; // clear slot
                    printf("\033[1;32mConsumer: Acquired mutex, consumed item P%d from slot "
                           "%d.\033[0m\n",
                           consumed, out);
                    out = (out + 1) % BUFFER_SIZE;

                    mutex++; // release lock
                    empty++;

                    if (prod_blocked)
                    {
                        prod_blocked = 0;
                        printf("\033[1;32mConsumer: Signal(empty_sem) sent. Producer is now "
                               "UNBLOCKED.\033[0m\n");
                    }
                }
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            printf("\nEnter number of simulation steps to run automatically (1 to 20): ");
            int steps;
            int step_status = safe_input_int(&steps, "", 1, 20);
            if (step_status != 1)
                continue;

            srand((unsigned int)time(NULL));

            for (int s = 0; s < steps; s++)
            {
                if (!is_instant())
                {
                    clear_screen();
                }
                printf("\n\033[1;36m=== Auto-Simulation (Step %d of %d) ===\033[0m\n", s + 1,
                       steps);
                display_buffer_state(buffer, in, out, mutex, empty, full, prod_blocked,
                                     cons_blocked);

                int act = rand() % 2; // 0 = produce, 1 = consume
                if (act == 0)
                {
                    printf("\n\033[1;33m[Simulated Event: Producer attempts action]\033[0m\n");
                    if (empty == 0)
                    {
                        prod_blocked = 1;
                        printf("\033[1;31mProducer: Buffer full. Blocked.\033[0m\n");
                    }
                    else
                    {
                        prod_blocked = 0;
                        empty--;
                        buffer[in] = item_counter++;
                        printf("\033[1;32mProducer: Placed item P%d at slot %d.\033[0m\n",
                               buffer[in], in);
                        in = (in + 1) % BUFFER_SIZE;
                        full++;
                        if (cons_blocked)
                        {
                            cons_blocked = 0;
                            printf("\033[1;32mProducer: Unblocked Consumer.\033[0m\n");
                        }
                    }
                }
                else
                {
                    printf("\n\033[1;33m[Simulated Event: Consumer attempts action]\033[0m\n");
                    if (full == 0)
                    {
                        cons_blocked = 1;
                        printf("\033[1;31mConsumer: Buffer empty. Blocked.\033[0m\n");
                    }
                    else
                    {
                        cons_blocked = 0;
                        full--;
                        int consumed = buffer[out];
                        buffer[out] = 0;
                        printf("\033[1;32mConsumer: Consumed item P%d from slot %d.\033[0m\n",
                               consumed, out);
                        out = (out + 1) % BUFFER_SIZE;
                        empty++;
                        if (prod_blocked)
                        {
                            prod_blocked = 0;
                            printf("\033[1;32mConsumer: Unblocked Producer.\033[0m\n");
                        }
                    }
                }

                sleep_seconds(1.2f);
            }
            printf("\nAuto-simulation finished. Press Enter to continue...");
            getchar();
        }
        else if (choice == 4)
        {
            for (int i = 0; i < BUFFER_SIZE; i++)
                buffer[i] = 0;
            in = 0;
            out = 0;
            mutex = 1;
            empty = BUFFER_SIZE;
            full = 0;
            prod_blocked = 0;
            cons_blocked = 0;
            printf("\nBuffer has been reset successfully!\n");
            sleep_seconds(0.8f);
        }
    }
}
