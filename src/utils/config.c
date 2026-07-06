#include "config.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include <stdio.h>

// Global static variables to hold the state
static int current_delay_seconds = 2;
static const char* speed_name = "Normal (2.0s)";

#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) void algorithm_step_hook(const char* event_msg)
{
    (void)event_msg;
}
#endif

void set_animation_speed(int choice)
{
    switch (choice)
    {
        case 1:
            current_delay_seconds = 3;
            speed_name = "Slow (3.0s)";
            break;
        case 2:
            current_delay_seconds = 2;
            speed_name = "Normal (2.0s)";
            break;
        case 3:
            current_delay_seconds = 1;
            speed_name = "Fast (1.0s)";
            break;
        case 4:
            current_delay_seconds = 0;
            speed_name = "Instant (0.0s)";
            break;
        default:
            current_delay_seconds = 2;
            speed_name = "Normal (2.0s)";
            break;
    }
}

void print_current_speed(void)
{
    printf("Current Speed: %s\n", speed_name);
}

void dynamic_sleep(void)
{
    if (!is_terminal_interactive())
    {
        return;
    }
    if (current_delay_seconds > 0)
    {
        sleep_seconds(current_delay_seconds);
    }
    // If it's 0 (Instant), skip sleeping entirely!
}

void settings_menu_demo(void)
{
    int speed_choice;

    printf("\n===================================\n");
    printf("  Global Animation Speed Settings\n");
    printf("===================================\n");
    print_current_speed();
    printf("\nSelect a new animation speed:\n");
    printf("1. Slow (3.0s)\n");
    printf("2. Normal (2.0s)\n");
    printf("3. Fast (1.0s)\n");
    printf("4. Instant (0.0s - Skip animations)\n");

    int status = safe_input_int(&speed_choice, "Choice: ", 1, 4);

    if (status == INPUT_EXIT_SIGNAL)
        return;
    if (status == 1)
    {
        set_animation_speed(speed_choice);
        printf("\n[Success] ");
        print_current_speed();
        dynamic_sleep(); // Brief pause so they can read the success message
    }
}

int is_instant(void)
{
    if (!is_terminal_interactive())
    {
        return 1;
    }
    return (current_delay_seconds == 0) ? 1 : 0;
}