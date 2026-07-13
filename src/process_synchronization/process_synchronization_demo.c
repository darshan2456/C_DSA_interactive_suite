#include "display_header.h"
#include "process_synchronization.h"
#include "safe_input.h"
#include <stdio.h>

void process_synchronization_demo(void)
{
    while (1)
    {
        display_header("Process Synchronization");

        int choice;
        int status = safe_input_int(&choice,
                                    "\n\n--- Process Synchronization Demos ---\n"
                                    "1. Producer-Consumer Problem (Bounded Buffer)\n"
                                    "2. Dining Philosophers Problem\n"
                                    "3. Peterson's Algorithm (2-Process Mutual Exclusion)\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 3);

        if (status == INPUT_EXIT_SIGNAL || choice == -1)
        {
            printf("\nExiting Process Synchronization demos....\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                producer_consumer_demo();
                break;
            case 2:
                dining_philosophers_demo();
                break;
            case 3:
                petersons_algorithm_demo();
                break;
        }
    }
}
