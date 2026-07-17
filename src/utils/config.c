#include "config.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>

#include <unistd.h>

int is_terminal_interactive(void)
{
    return isatty(1);
}

// Global static variables to hold the state
static int current_delay_seconds = 2;
static const char* speed_name = "Normal (2.0s)";

static int step_mode_active = 0;
static int paused = 0;

void set_step_mode(int active)
{
    step_mode_active = active;
}

int get_step_mode(void)
{
    return step_mode_active;
}

void set_paused(int p)
{
    paused = p;
}

int get_paused(void)
{
    return paused;
}

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
    while (1)
    {
        printf("\n===================================\n");
        printf("        Settings & Debugger        \n");
        printf("===================================\n");
        print_current_speed();
        printf("Debugger Step Mode: %s\n", get_step_mode() ? "ON" : "OFF");
        printf("\nOptions:\n");
        printf("1. Set Animation Speed\n");
        printf("2. Toggle Debugger Step Mode\n");
        printf("Select option (or enter -1 to go back): \n");

        int option;
        int status = safe_input_int(&option, "Choice: ", 1, 2);

        if (status == INPUT_EXIT_SIGNAL)
            return;

        if (status == 1)
        {
            if (option == 1)
            {
                int speed_choice;
                printf("\nSelect a new animation speed:\n");
                printf("1. Slow (3.0s)\n");
                printf("2. Normal (2.0s)\n");
                printf("3. Fast (1.0s)\n");
                printf("4. Instant (0.0s - Skip animations)\n");

                status = safe_input_int(&speed_choice, "Choice: ", 1, 4);
                if (status == 1)
                {
                    set_animation_speed(speed_choice);
                    printf("\n[Success] ");
                    print_current_speed();
                }
            }
            else if (option == 2)
            {
                int new_mode = !get_step_mode();
                set_step_mode(new_mode);
                set_paused(new_mode);
                printf("\n[Success] Debugger Step Mode set to: %s\n",
                       new_mode ? "ON (Auto-Paused)" : "OFF");
            }
        }
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

void init_windows_console(void) {}

void set_telemetry_trace_enabled(int enabled)
{
    set_telemetry_enabled(enabled ? true : false);
}

int is_telemetry_trace_enabled(void)
{
    return is_telemetry_enabled() ? 1 : 0;
}
