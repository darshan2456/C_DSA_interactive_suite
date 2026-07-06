#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "process_synchronization.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char logs[3][128] = {"", "", ""};

static void add_log(const char* message)
{
    memmove(logs[0], logs[1], sizeof(logs[0]));
    memmove(logs[1], logs[2], sizeof(logs[1]));
    snprintf(logs[2], sizeof(logs[2]), "%s", message);
}

static void print_cell_p0(int step, int active)
{
    const char* label = "";
    switch (step)
    {
        case 0:
            label = "l1: Non-Critical Sec";
            break;
        case 1:
            label = "l2: readyA := true";
            break;
        case 2:
            label = "l3: turn := B";
            break;
        case 3:
            label = "l4: await(!readyB||t=A)";
            break;
        case 4:
            label = "l5: Critical Section";
            break;
        case 5:
            label = "l6: readyA := false";
            break;
    }

    if (active)
    {
        if (step == 4) // CS
            printf(" \033[1;32m-> %-21s\033[0m  ", label);
        else if (step == 3) // Busy-Wait
            printf(" \033[1;33m-> %-21s\033[0m  ", label);
        else
            printf(" \033[1;36m-> %-21s\033[0m  ", label);
    }
    else
    {
        printf("    %-21s  ", label);
    }
}

static void print_cell_p1(int step, int active)
{
    const char* label = "";
    switch (step)
    {
        case 0:
            label = "m1: Non-Critical Sec";
            break;
        case 1:
            label = "m2: readyB := true";
            break;
        case 2:
            label = "m3: turn := A";
            break;
        case 3:
            label = "m4: await(!readyA||t=B)";
            break;
        case 4:
            label = "m5: Critical Section";
            break;
        case 5:
            label = "m6: readyB := false";
            break;
    }

    if (active)
    {
        if (step == 4) // CS
            printf(" \033[1;32m-> %-22s\033[0m  ", label);
        else if (step == 3) // Busy-Wait
            printf(" \033[1;33m-> %-22s\033[0m  ", label);
        else
            printf(" \033[1;36m-> %-22s\033[0m  ", label);
    }
    else
    {
        printf("    %-22s  ", label);
    }
}

static void display_state_card(const int* flag, int turn, const int* pc)
{
    printf("\n\033[1;34m┌────────────────────────────────────────────────────────┐\033[0m");
    printf("\n\033[1;34m│                 PETERSON'S STATE CARD                  │\033[0m");
    printf("\n\033[1;34m├───────────────────────────┬────────────────────────────┤\033[0m");
    printf("\n\033[1;34m│ Process 0 (A) Steps       │ Process 1 (B) Steps        │\033[0m");
    printf("\n\033[1;34m├───────────────────────────┼────────────────────────────┤\033[0m");

    for (int s = 0; s < 6; s++)
    {
        printf("\n│");
        print_cell_p0(s, pc[0] == s);
        printf("│");
        print_cell_p1(s, pc[1] == s);
        printf("│");
    }

    printf("\n\033[1;34m├───────────────────────────┴────────────────────────────┤\033[0m");
    printf("\n│ Shared Variables:                                      │");
    printf("\n│   flag[0] = %d (readyA)                                 │", flag[0]);
    printf("\n│   flag[1] = %d (readyB)                                 │", flag[1]);
    printf("\n│   turn    = %d                                           │", turn);
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m");
    printf("\n│ Synchronization Logic (Awaiting Condition):            │");

    // Condition to block P0: flag[1] == 1 && turn == 1
    int p0_blocked = (flag[1] && turn == 1);
    printf("\n│   P0 Wait: flag[1] && turn == 1 (%d&&%d) => ", flag[1], turn == 1);
    if (p0_blocked)
        printf("\033[1;31mBLOCKED      \033[0m│");
    else
        printf("\033[1;32mFREE TO ENTER\033[0m│");

    // Condition to block P1: flag[0] == 1 && turn == 0
    int p1_blocked = (flag[0] && turn == 0);
    printf("\n│   P1 Wait: flag[0] && turn == 0 (%d&&%d) => ", flag[0], turn == 0);
    if (p1_blocked)
        printf("\033[1;31mBLOCKED      \033[0m│");
    else
        printf("\033[1;32mFREE TO ENTER\033[0m│");

    printf("\n\033[1;34m└────────────────────────────────────────────────────────┘\033[0m\n");
}

static void step_process(int i, int* flag, int* turn, int* pc)
{
    int j = 1 - i;
    char msg[128];

    switch (pc[i])
    {
        case 0: // l1/m1: non-critical section
            pc[i] = 1;
            snprintf(msg, sizeof(msg), "P%d left non-critical section, ready to request entry.", i);
            add_log(msg);
            break;
        case 1: // l2/m2: ready := true
            flag[i] = 1;
            pc[i] = 2;
            snprintf(msg, sizeof(msg), "P%d set flag[%d] = 1 (ready%s = true).", i, i,
                     i == 0 ? "A" : "B");
            add_log(msg);
            break;
        case 2: // l3/m3: turn := other
            *turn = j;
            pc[i] = 3;
            snprintf(msg, sizeof(msg), "P%d set turn = %d (turn = %s).", i, j, j == 0 ? "A" : "B");
            add_log(msg);
            break;
        case 3: // l4/m4: await (ready_other == false || turn == self)
            if (flag[j] == 0 || *turn == i)
            {
                pc[i] = 4;
                snprintf(msg, sizeof(msg), "P%d evaluated await (free): entered CS! 🎉", i);
                add_log(msg);
            }
            else
            {
                snprintf(msg, sizeof(msg), "P%d evaluated await (busy): must busy-wait.", i);
                add_log(msg);
            }
            break;
        case 4: // l5/m5: critical section
            pc[i] = 5;
            snprintf(msg, sizeof(msg), "P%d finished critical section, ready to release flag.", i);
            add_log(msg);
            break;
        case 5: // l6/m6: ready := false
            flag[i] = 0;
            pc[i] = 0;
            snprintf(msg, sizeof(msg), "P%d reset flag[%d] = 0 (ready%s = false).", i, i,
                     i == 0 ? "A" : "B");
            add_log(msg);
            break;
    }
}

void petersons_algorithm_demo(void)
{
    if (!is_instant())
    {
        clear_screen();
    }
    int flag[2] = {0, 0};
    int turn = 0;
    int pc[2] = {0, 0};

    // Reset logs
    for (int i = 0; i < 3; i++)
    {
        strcpy(logs[i], "");
    }
    add_log("Peterson's Algorithm Simulation Initialized.");

    while (1)
    {
        clear_screen();
        printf("\n\033[1;36m=== PETERSON'S ALGORITHM SIMULATOR ===\033[0m\n");
        printf("This simulator models Peterson's classic software-based solution to\n");
        printf("the mutual exclusion problem for two cooperative processes.\n");

        display_state_card(flag, turn, pc);

        printf("\n\033[1;35mRecent Activity Logs:\033[0m\n");
        for (int i = 0; i < 3; i++)
        {
            if (strlen(logs[i]) > 0)
            {
                printf(" - %s\n", logs[i]);
            }
            else
            {
                printf(" -\n");
            }
        }

        // Safety verification check: verify mutual exclusion is never violated
        if (pc[0] == 4 && pc[1] == 4)
        {
            printf("\n\033[1;31m⚠️ MUTUAL EXCLUSION VIOLATED! (This should be "
                   "impossible)\033[0m\n");
        }

        printf("\nOptions:\n");
        printf("1. Step Process 0 (P0 / A)\n");
        printf("2. Step Process 1 (P1 / B)\n");
        printf("3. Auto-simulate random steps\n");
        printf("4. Reset simulation table\n");
        printf("Enter option (-1 to return to main menu): ");
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
            step_process(0, flag, &turn, pc);
        }
        else if (choice == 2)
        {
            step_process(1, flag, &turn, pc);
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
                clear_screen();
                printf("\n\033[1;36m=== Auto-Simulation (Step %d of %d) ===\033[0m\n", s + 1,
                       steps);
                display_state_card(flag, turn, pc);

                printf("\n\033[1;35mRecent Activity Logs:\033[0m\n");
                for (int i = 0; i < 3; i++)
                {
                    if (strlen(logs[i]) > 0)
                    {
                        printf(" - %s\n", logs[i]);
                    }
                    else
                    {
                        printf(" -\n");
                    }
                }

                // Choose a random process (P0 or P1) to step
                int p_id = rand() % 2;
                step_process(p_id, flag, &turn, pc);

                sleep_seconds(1.2f);
            }
            printf("\nAuto-simulation finished. Press Enter to continue...");
            getchar();
        }
        else if (choice == 4)
        {
            flag[0] = 0;
            flag[1] = 0;
            turn = 0;
            pc[0] = 0;
            pc[1] = 0;
            for (int i = 0; i < 3; i++)
                strcpy(logs[i], "");
            add_log("Simulation table reset successfully.");
            printf("\nSimulation state has been reset!\n");
            sleep_seconds(0.8f);
        }
    }
}
