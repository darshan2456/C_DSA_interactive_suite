#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "process_synchronization.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum
{
    THINKING,
    HUNGRY,
    EATING
} PhilosopherState;

static char logs[3][128] = {"", "", ""};

static void add_log(const char* message)
{
    // Shift logs up
    memmove(logs[0], logs[1], sizeof(logs[0]));
    memmove(logs[1], logs[2], sizeof(logs[1]));
    snprintf(logs[2], sizeof(logs[2]), "%s", message);
}

static void get_phil_box(PhilosopherState state, int id, char* box_buf)
{
    const char* state_color;
    const char* state_name;
    switch (state)
    {
        case EATING:
            state_color = "\033[1;32m"; // Green
            state_name = "EATING  ";
            break;
        case HUNGRY:
            state_color = "\033[1;33m"; // Yellow
            state_name = "HUNGRY  ";
            break;
        default:
            state_color = "\033[1;36m"; // Cyan
            state_name = "THINKING";
            break;
    }
    // Printable length inside box is exactly 12 characters, e.g. "P0: THINKING" or "P1: EATING  "
    snprintf(box_buf, 64, "%sP%d: %s\033[0m", state_color, id, state_name);
}

static void get_chopstick_str(int id, const int* chopsticks, char* buf)
{
    if (chopsticks[id] == -1)
    {
        snprintf(buf, 64, "\033[0;37mC%d: FREE\033[0m", id);
    }
    else
    {
        snprintf(buf, 64, "\033[1;31mC%d: P%d  \033[0m", id, chopsticks[id]);
    }
}

static void display_table_state(const int* chopsticks, const PhilosopherState* phil_states,
                                int strategy)
{
    char box0[64], box1[64], box2[64], box3[64], box4[64];
    char c0[64], c1[64], c2[64], c3[64], c4[64];

    get_phil_box(phil_states[0], 0, box0);
    get_phil_box(phil_states[1], 1, box1);
    get_phil_box(phil_states[2], 2, box2);
    get_phil_box(phil_states[3], 3, box3);
    get_phil_box(phil_states[4], 4, box4);

    get_chopstick_str(0, chopsticks, c0);
    get_chopstick_str(1, chopsticks, c1);
    get_chopstick_str(2, chopsticks, c2);
    get_chopstick_str(3, chopsticks, c3);
    get_chopstick_str(4, chopsticks, c4);

    const char* strategy_name = "Unknown";
    if (strategy == 0)
        strategy_name = "Naive (Allows Deadlock)";
    else if (strategy == 1)
        strategy_name = "Asymmetric (Odd/Even ID)";
    else if (strategy == 2)
        strategy_name = "Arbitrator (State Check)";

    printf("\n\033[1;34m┌────────────────────────────────────────────────────────┐\033[0m");
    printf("\n\033[1;34m│               DINING PHILOSOPHERS TABLE                │\033[0m");
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m");
    printf("\n│ Strategy: %-44s │", strategy_name);
    printf("\n\033[1;34m├────────────────────────────────────────────────────────┤\033[0m\n");

    printf("                      ┌──────────────┐\n");
    printf("                      │ %s │\n", box0);
    printf("                      └──────────────┘\n");
    printf("                %s          %s\n", c0, c1);
    printf("                       │              │\n");
    printf("        ┌──────────────┴───┐      ┌───┴──────────────┐\n");
    printf("        │   %s   │      │   %s   │\n", box4, box1);
    printf("        └──────────────────┘      └──────────────────┘\n");
    printf("           %s                  %s\n", c4, c2);
    printf("               │                              │\n");
    printf("        ┌──────┴───────────┐      ┌───────────┴──────┐\n");
    printf("        │   %s   │──────│   %s   │\n", box3, box2);
    printf("        └──────────────────┘      └──────────────────┘\n");
    printf("                       %s\n", c3);

    printf("\033[1;34m└────────────────────────────────────────────────────────┘\033[0m\n");
}

static void test_arbitrator(int i, int* chopsticks, PhilosopherState* phil_states)
{
    int left_neighbor = (i + 4) % 5;
    int right_neighbor = (i + 1) % 5;
    if (phil_states[i] == HUNGRY && phil_states[left_neighbor] != EATING &&
        phil_states[right_neighbor] != EATING)
    {
        phil_states[i] = EATING;
        chopsticks[i] = i;
        chopsticks[right_neighbor] = i;

        char msg[128];
        snprintf(msg, sizeof(msg), "Arbitrator: P%d acquired chopsticks %d & %d. Starts EATING!", i,
                 i, right_neighbor);
        add_log(msg);
    }
}

static void attempt_eat(int i, int strategy, int* chopsticks, PhilosopherState* phil_states)
{
    int left = i;
    int right = (i + 1) % 5;
    char msg[128];

    if (strategy == 0)
    { // Naive
        if (chopsticks[left] == i)
        {
            // Already holds left, try right
            if (chopsticks[right] == -1)
            {
                chopsticks[right] = i;
                phil_states[i] = EATING;
                snprintf(msg, sizeof(msg), "P%d acquired Right Chopstick %d and is now EATING! 🎉",
                         i, right);
                add_log(msg);
            }
            else
            {
                snprintf(msg, sizeof(msg), "P%d holds Left C%d; Right C%d is busy (held by P%d).",
                         i, left, right, chopsticks[right]);
                add_log(msg);
            }
        }
        else if (chopsticks[left] == -1)
        {
            chopsticks[left] = i;
            snprintf(msg, sizeof(msg), "P%d acquired Left Chopstick %d.", i, left);
            add_log(msg);

            // Attempt right in the same step
            if (chopsticks[right] == -1)
            {
                chopsticks[right] = i;
                phil_states[i] = EATING;
                snprintf(msg, sizeof(msg), "P%d acquired Right Chopstick %d and is now EATING! 🎉",
                         i, right);
                add_log(msg);
            }
            else
            {
                snprintf(msg, sizeof(msg), "P%d is waiting for Right Chopstick %d.", i, right);
                add_log(msg);
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "P%d cannot acquire Left Chopstick %d (held by P%d).", i,
                     left, chopsticks[left]);
            add_log(msg);
        }
    }
    else if (strategy == 1)
    { // Asymmetric
        if (i % 2 == 0)
        { // Even ID: Left first, then Right
            if (chopsticks[left] == i)
            {
                if (chopsticks[right] == -1)
                {
                    chopsticks[right] = i;
                    phil_states[i] = EATING;
                    snprintf(msg, sizeof(msg),
                             "P%d acquired Right Chopstick %d and is now EATING! 🎉", i, right);
                    add_log(msg);
                }
                else
                {
                    snprintf(msg, sizeof(msg), "P%d holds Left C%d; Right C%d is held by P%d.", i,
                             left, right, chopsticks[right]);
                    add_log(msg);
                }
            }
            else if (chopsticks[left] == -1)
            {
                chopsticks[left] = i;
                snprintf(msg, sizeof(msg), "P%d (Even) acquired Left Chopstick %d.", i, left);
                add_log(msg);

                if (chopsticks[right] == -1)
                {
                    chopsticks[right] = i;
                    phil_states[i] = EATING;
                    snprintf(msg, sizeof(msg),
                             "P%d acquired Right Chopstick %d and is now EATING! 🎉", i, right);
                    add_log(msg);
                }
                else
                {
                    snprintf(msg, sizeof(msg), "P%d is waiting for Right Chopstick %d.", i, right);
                    add_log(msg);
                }
            }
            else
            {
                snprintf(msg, sizeof(msg), "P%d cannot acquire Left Chopstick %d (held by P%d).", i,
                         left, chopsticks[left]);
                add_log(msg);
            }
        }
        else
        { // Odd ID: Right first, then Left
            if (chopsticks[right] == i)
            {
                if (chopsticks[left] == -1)
                {
                    chopsticks[left] = i;
                    phil_states[i] = EATING;
                    snprintf(msg, sizeof(msg),
                             "P%d acquired Left Chopstick %d and is now EATING! 🎉", i, left);
                    add_log(msg);
                }
                else
                {
                    snprintf(msg, sizeof(msg), "P%d holds Right C%d; Left C%d is held by P%d.", i,
                             right, left, chopsticks[left]);
                    add_log(msg);
                }
            }
            else if (chopsticks[right] == -1)
            {
                chopsticks[right] = i;
                snprintf(msg, sizeof(msg), "P%d (Odd) acquired Right Chopstick %d.", i, right);
                add_log(msg);

                if (chopsticks[left] == -1)
                {
                    chopsticks[left] = i;
                    phil_states[i] = EATING;
                    snprintf(msg, sizeof(msg),
                             "P%d acquired Left Chopstick %d and is now EATING! 🎉", i, left);
                    add_log(msg);
                }
                else
                {
                    snprintf(msg, sizeof(msg), "P%d is waiting for Left Chopstick %d.", i, left);
                    add_log(msg);
                }
            }
            else
            {
                snprintf(msg, sizeof(msg), "P%d cannot acquire Right Chopstick %d (held by P%d).",
                         i, right, chopsticks[right]);
                add_log(msg);
            }
        }
    }
    else if (strategy == 2)
    { // Arbitrator
        test_arbitrator(i, chopsticks, phil_states);
    }
}

static void trigger_philosopher(int i, int strategy, int* chopsticks, PhilosopherState* phil_states)
{
    char msg[128];
    if (phil_states[i] == EATING)
    {
        if (strategy == 2)
        { // Arbitrator
            phil_states[i] = THINKING;
            chopsticks[i] = -1;
            chopsticks[(i + 1) % 5] = -1;
            snprintf(msg, sizeof(msg), "P%d finished eating and is now THINKING.", i);
            add_log(msg);

            // Test neighbors to unblock them
            test_arbitrator((i + 4) % 5, chopsticks, phil_states);
            test_arbitrator((i + 1) % 5, chopsticks, phil_states);
        }
        else
        {
            chopsticks[i] = -1;
            chopsticks[(i + 1) % 5] = -1;
            phil_states[i] = THINKING;
            snprintf(msg, sizeof(msg), "P%d released chopsticks %d & %d, now THINKING.", i, i,
                     (i + 1) % 5);
            add_log(msg);
        }
    }
    else if (phil_states[i] == THINKING)
    {
        phil_states[i] = HUNGRY;
        snprintf(msg, sizeof(msg), "P%d became HUNGRY.", i);
        add_log(msg);
        attempt_eat(i, strategy, chopsticks, phil_states);
    }
    else if (phil_states[i] == HUNGRY)
    {
        attempt_eat(i, strategy, chopsticks, phil_states);
    }
}

void dining_philosophers_demo(void)
{
    int chopsticks[5] = {-1, -1, -1, -1, -1};
    PhilosopherState phil_states[5] = {THINKING, THINKING, THINKING, THINKING, THINKING};
    int strategy = 0; // 0 = Naive, 1 = Asymmetric, 2 = Arbitrator

    // Reset logs
    for (int i = 0; i < 3; i++)
    {
        strcpy(logs[i], "");
    }
    add_log("Dining Philosophers Simulation Initialized.");

    while (1)
    {
        if (!is_instant())
        {
            clear_screen();
        }
        printf("\n\033[1;36m=== DINING PHILOSOPHERS PROBLEM SIMULATOR ===\033[0m\n");
        printf(
            "This simulation models the classic Dining Philosophers synchronization challenge.\n");

        display_table_state(chopsticks, phil_states, strategy);

        // Check for deadlock
        int hungry_count = 0;
        int eating_count = 0;
        int chopsticks_held_count = 0;
        for (int p = 0; p < 5; p++)
        {
            if (phil_states[p] == HUNGRY)
                hungry_count++;
            if (phil_states[p] == EATING)
                eating_count++;
        }
        for (int c = 0; c < 5; c++)
        {
            if (chopsticks[c] != -1)
                chopsticks_held_count++;
        }

        int is_deadlocked = (hungry_count == 5 && eating_count == 0 && chopsticks_held_count == 5);
        if (is_deadlocked)
        {
            printf("\n\033[1;31m⚠️ DEADLOCK DETECTED!\033[0m\n");
            printf("\033[1;31m- All 5 philosophers are HUNGRY and holding exactly 1 "
                   "chopstick.\033[0m\n");
            printf("\033[1;31m- Circular wait condition is met. No philosopher can "
                   "progress.\033[0m\n");
            printf("\033[1;33mTip: Change strategy (Option 3) or reset simulation (Option 4) to "
                   "resolve.\033[0m\n");
        }

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

        printf("\nOptions:\n");
        printf("1. Trigger Philosopher state change (0-4)\n");
        printf("2. Auto-simulate random steps\n");
        printf("3. Change Deadlock Prevention Strategy\n");
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
            printf("\nEnter Philosopher ID (0-4): ");
            int p_id;
            int p_status = safe_input_int(&p_id, "", 0, 4);
            if (p_status == 1)
            {
                trigger_philosopher(p_id, strategy, chopsticks, phil_states);
            }
        }
        else if (choice == 2)
        {
            printf("\nEnter number of simulation steps to run automatically (1 to 20): ");
            int steps;
            int step_status = safe_input_int(&steps, "", 1, 20);
            if (step_status != 1)
                continue;

            srand((unsigned int)time(NULL));

            for (int s = 0; s < steps; s++)
            {
                // Check if already deadlocked; if so, abort auto-simulate to let user see deadlock
                hungry_count = 0;
                eating_count = 0;
                chopsticks_held_count = 0;
                for (int p = 0; p < 5; p++)
                {
                    if (phil_states[p] == HUNGRY)
                        hungry_count++;
                    if (phil_states[p] == EATING)
                        eating_count++;
                }
                for (int c = 0; c < 5; c++)
                {
                    if (chopsticks[c] != -1)
                        chopsticks_held_count++;
                }
                if (hungry_count == 5 && eating_count == 0 && chopsticks_held_count == 5)
                {
                    add_log("Auto-simulation stopped due to Deadlock.");
                    break;
                }

                if (!is_instant())
                {
                    clear_screen();
                }
                printf("\n\033[1;36m=== Auto-Simulation (Step %d of %d) ===\033[0m\n", s + 1,
                       steps);
                display_table_state(chopsticks, phil_states, strategy);

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

                // Choose a random philosopher and trigger state transition
                int p_id = rand() % 5;
                trigger_philosopher(p_id, strategy, chopsticks, phil_states);

                sleep_seconds(1.2f);
            }
            printf("\nAuto-simulation finished. Press Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            if (!is_instant())
            {
                clear_screen();
            }
            printf("\n\033[1;36m=== Choose Deadlock Prevention Strategy ===\033[0m\n");
            printf("1. Naive (Philosophers pick left then right; allows deadlock)\n");
            printf("2. Asymmetric (Even philosophers pick left first; odd pick right first; "
                   "prevents deadlock)\n");
            printf("3. Arbitrator (Philosophers only acquire chopsticks if both are free; prevents "
                   "deadlock)\n");
            printf("\nEnter strategy (1-3): ");

            int strat_choice;
            int strat_status = safe_input_int(&strat_choice, "", 1, 3);
            if (strat_status == 1)
            {
                strategy = strat_choice - 1;
                // Reset table on strategy change
                for (int c = 0; c < 5; c++)
                    chopsticks[c] = -1;
                for (int p = 0; p < 5; p++)
                    phil_states[p] = THINKING;
                for (int i = 0; i < 3; i++)
                    strcpy(logs[i], "");
                add_log("Strategy changed. Simulation table reset.");
            }
        }
        else if (choice == 4)
        {
            for (int c = 0; c < 5; c++)
                chopsticks[c] = -1;
            for (int p = 0; p < 5; p++)
                phil_states[p] = THINKING;
            for (int i = 0; i < 3; i++)
                strcpy(logs[i], "");
            add_log("Simulation table reset successfully.");
            printf("\nSimulation state has been reset!\n");
            sleep_seconds(0.8f);
        }
    }
}
