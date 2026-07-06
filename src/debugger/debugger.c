#include "debugger.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void debugger_init(DebuggerContext* ctx)
{
    if (!ctx) return;
    memset(ctx, 0, sizeof(DebuggerContext));
    ctx->state = DEBUG_RUN;
    ctx->step_mode = 0;
    ctx->auto_continue = 1;
    ctx->total_steps = 0;
    ctx->num_breakpoints = 0;
    ctx->event_count = 0;
}

void debugger_destroy(DebuggerContext* ctx)
{
    (void)ctx;
}

void debugger_set_mode(DebuggerContext* ctx, DebugState state)
{
    if (!ctx) return;
    ctx->state = state;
}

DebugState debugger_get_mode(DebuggerContext* ctx)
{
    if (!ctx) return DEBUG_RUN;
    return ctx->state;
}

void debugger_enable_step_mode(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->step_mode = 1;
    ctx->auto_continue = 0;
}

void debugger_disable_step_mode(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->step_mode = 0;
    ctx->auto_continue = 1;
    ctx->state = DEBUG_RUN;
}

int debugger_is_step_mode(DebuggerContext* ctx)
{
    if (!ctx) return 0;
    return ctx->step_mode;
}

void debugger_pause(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->state = DEBUG_PAUSE;
}

void debugger_resume(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->state = DEBUG_RUN;
}

int debugger_is_paused(DebuggerContext* ctx)
{
    if (!ctx) return 0;
    return ctx->state == DEBUG_PAUSE;
}

void debugger_step(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->state = DEBUG_STEP;
}

void debugger_add_breakpoint(DebuggerContext* ctx, unsigned int step)
{
    if (!ctx || ctx->num_breakpoints >= MAX_EVENT_HISTORY) return;
    if (debugger_has_breakpoint(ctx, step)) return;
    ctx->breakpoints[ctx->num_breakpoints++] = step;
}

void debugger_remove_breakpoint(DebuggerContext* ctx, unsigned int step)
{
    if (!ctx) return;
    for (int i = 0; i < ctx->num_breakpoints; i++)
    {
        if (ctx->breakpoints[i] == step)
        {
            for (int j = i; j < ctx->num_breakpoints - 1; j++)
            {
                ctx->breakpoints[j] = ctx->breakpoints[j + 1];
            }
            ctx->num_breakpoints--;
            return;
        }
    }
}

void debugger_clear_breakpoints(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->num_breakpoints = 0;
}

int debugger_has_breakpoint(DebuggerContext* ctx, unsigned int step)
{
    if (!ctx) return 0;
    for (int i = 0; i < ctx->num_breakpoints; i++)
    {
        if (ctx->breakpoints[i] == step) return 1;
    }
    return 0;
}

const char* debugger_event_type_str(EventType type)
{
    switch (type)
    {
        case EVT_COMPARE:  return "COMPARE";
        case EVT_SWAP:     return "SWAP";
        case EVT_INSERT:    return "INSERT";
        case EVT_DELETE:    return "DELETE";
        case EVT_VISIT:     return "VISIT";
        case EVT_ENQUEUE:   return "ENQUEUE";
        case EVT_DEQUEUE:   return "DEQUEUE";
        case EVT_RELAX:     return "RELAX";
        case EVT_UPDATE:    return "UPDATE";
        case EVT_MARK:     return "MARK";
        case EVT_CUSTOM:   return "CUSTOM";
        default:           return "UNKNOWN";
    }
}

const char* debugger_state_str(DebugState state)
{
    switch (state)
    {
        case DEBUG_STEP:  return "STEP";
        case DEBUG_PAUSE: return "PAUSE";
        case DEBUG_RUN:   return "RUN";
        default:          return "UNKNOWN";
    }
}

void debugger_hook(DebuggerContext* ctx, const char* event_msg)
{
    if (!ctx || !event_msg) return;

    ctx->total_steps++;

    DebugEvent evt;
    evt.type = EVT_CUSTOM;
    evt.data1 = 0;
    evt.data2 = 0;
    evt.step_number = ctx->total_steps;
    strncpy(evt.message, event_msg, MAX_EVENT_MSG_LEN - 1);
    evt.message[MAX_EVENT_MSG_LEN - 1] = '\0';

    if (ctx->event_count < MAX_EVENT_HISTORY)
    {
        ctx->event_history[ctx->event_count++] = evt;
    }
    else
    {
        for (int i = 0; i < MAX_EVENT_HISTORY - 1; i++)
        {
            ctx->event_history[i] = ctx->event_history[i + 1];
        }
        ctx->event_history[MAX_EVENT_HISTORY - 1] = evt;
    }

    if (ctx->auto_continue) return;

    if (debugger_has_breakpoint(ctx, ctx->total_steps))
    {
        debugger_pause(ctx);
    }

    if (ctx->state == DEBUG_PAUSE || ctx->state == DEBUG_STEP)
    {
        printf("\n  [BREAKPOINT at step %u] %s\n", ctx->total_steps, event_msg);
        debugger_print_status(ctx);
        debugger_wait_for_input(ctx);
    }
}

void debugger_hook_ex(DebuggerContext* ctx, EventType type, int data1, int data2, const char* format, ...)
{
    if (!ctx) return;

    char msg[MAX_EVENT_MSG_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    ctx->total_steps++;

    DebugEvent evt;
    evt.type = type;
    evt.data1 = data1;
    evt.data2 = data2;
    evt.step_number = ctx->total_steps;
    strncpy(evt.message, msg, MAX_EVENT_MSG_LEN - 1);
    evt.message[MAX_EVENT_MSG_LEN - 1] = '\0';

    if (ctx->event_count < MAX_EVENT_HISTORY)
    {
        ctx->event_history[ctx->event_count++] = evt;
    }
    else
    {
        for (int i = 0; i < MAX_EVENT_HISTORY - 1; i++)
        {
            ctx->event_history[i] = ctx->event_history[i + 1];
        }
        ctx->event_history[MAX_EVENT_HISTORY - 1] = evt;
    }

    if (ctx->auto_continue) return;

    if (debugger_has_breakpoint(ctx, ctx->total_steps))
    {
        debugger_pause(ctx);
    }

    if (ctx->state == DEBUG_PAUSE || ctx->state == DEBUG_STEP)
    {
        printf("\n  [BREAKPOINT at step %u] [%s] %s\n", 
               ctx->total_steps, debugger_event_type_str(type), msg);
        debugger_print_status(ctx);
        debugger_wait_for_input(ctx);
    }
}

void debugger_print_event(DebugEvent* evt)
{
    if (!evt) return;
    printf("  [%4u] [%s] %s", 
           evt->step_number, 
           debugger_event_type_str(evt->type),
           evt->message);
    if (evt->data1 != 0 || evt->data2 != 0)
    {
        printf(" (d1=%d, d2=%d)", evt->data1, evt->data2);
    }
    printf("\n");
}

void debugger_print_history(DebuggerContext* ctx)
{
    if (!ctx) return;

    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||                  EVENT HISTORY                        ||\n");
    printf("  +========================================================+\n");

    if (ctx->event_count == 0)
    {
        printf("  |  No events recorded yet.                               |\n");
    }
    else
    {
        int start = (ctx->event_count > 10) ? (ctx->event_count - 10) : 0;
        for (int i = start; i < ctx->event_count; i++)
        {
            debugger_print_event(&ctx->event_history[i]);
        }
    }
    printf("  +========================================================+\n");
    printf("\n");
}

void debugger_print_status(DebuggerContext* ctx)
{
    if (!ctx) return;

    printf("  +========================================================+\n");
    printf("  ||                   DEBUGGER STATUS                     ||\n");
    printf("  +========================================================+\n");
    printf("  |  State:          %-10s                         |\n", debugger_state_str(ctx->state));
    printf("  |  Step Mode:      %-10s                         |\n", ctx->step_mode ? "ON" : "OFF");
    printf("  |  Total Steps:    %-10u                         |\n", ctx->total_steps);
    printf("  |  Breakpoints:    %-10d                         |\n", ctx->num_breakpoints);
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Controls: [P]ause  [R]esume  [S]tep  [Q]uit  [H]istory\n");
}

void debugger_reset_counters(DebuggerContext* ctx)
{
    if (!ctx) return;
    ctx->total_steps = 0;
    ctx->event_count = 0;
    ctx->state = DEBUG_RUN;
}

unsigned int debugger_get_step_count(DebuggerContext* ctx)
{
    if (!ctx) return 0;
    return ctx->total_steps;
}

int debugger_wait_for_input(DebuggerContext* ctx)
{
    if (!ctx) return 0;

    printf("\n  Debugger waiting for input...\n");
    printf("  [P]ause  [R]esume  [S]tep  [Q]uit  [H]istory\n");
    printf("  > ");

    int ch = getchar();
    while (getchar() != '\n');

    switch (ch)
    {
        case 'p':
        case 'P':
            debugger_pause(ctx);
            printf("  [Paused]\n");
            break;
        case 'r':
        case 'R':
            debugger_resume(ctx);
            printf("  [Running]\n");
            break;
        case 's':
        case 'S':
            debugger_step(ctx);
            printf("  [Stepping]\n");
            break;
        case 'q':
        case 'Q':
            debugger_disable_step_mode(ctx);
            printf("  [Quit step mode - running at full speed]\n");
            break;
        case 'h':
        case 'H':
            debugger_print_history(ctx);
            break;
        default:
            break;
    }

    return ch;
}

static void demo_bubble_sort_with_debugger(DebuggerContext* ctx)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         BUBBLE SORT WITH STEPPER DEBUGGER              ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int arr[] = {64, 34, 25, 12, 22, 11, 90, 45, 33, 78};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("  Initial array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    debugger_hook(ctx, "Starting bubble sort");
    debugger_hook_ex(ctx, EVT_CUSTOM, n, 0, "Array size: %d", n);

    for (int i = 0; i < n - 1; i++)
    {
        debugger_hook_ex(ctx, EVT_CUSTOM, i, 0, "Outer loop iteration: %d", i);

        for (int j = 0; j < n - i - 1; j++)
        {
            debugger_hook_ex(ctx, EVT_COMPARE, j, j + 1, "Comparing arr[%d]=%d with arr[%d]=%d",
                           j, arr[j], j + 1, arr[j + 1]);

            if (arr[j] > arr[j + 1])
            {
                debugger_hook_ex(ctx, EVT_SWAP, j, j + 1, "Swapping arr[%d]=%d with arr[%d]=%d",
                               j, arr[j], j + 1, arr[j + 1]);

                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }

        debugger_hook_ex(ctx, EVT_MARK, n - i - 1, arr[n - i - 1], 
                        "Element %d is now in sorted position", arr[n - i - 1]);
    }

    debugger_hook(ctx, "Bubble sort completed");
    debugger_print_history(ctx);

    printf("  Sorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");
}

static void demo_linear_search_with_debugger(DebuggerContext* ctx)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         LINEAR SEARCH WITH STEPPER DEBUGGER            ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 23;

    printf("  Array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n  Target: %d\n\n", target);

    debugger_hook_ex(ctx, EVT_CUSTOM, target, 0, "Searching for value: %d", target);

    int found = -1;
    for (int i = 0; i < n; i++)
    {
        debugger_hook_ex(ctx, EVT_VISIT, i, arr[i], "Visiting index %d, value %d", i, arr[i]);

        if (arr[i] == target)
        {
            found = i;
            debugger_hook_ex(ctx, EVT_MARK, i, arr[i], "FOUND! Value %d at index %d", arr[i], i);
            break;
        }
    }

    if (found == -1)
    {
        debugger_hook(ctx, "Target not found in array");
    }

    debugger_print_history(ctx);
    printf("  Result: %s (index %d)\n\n", 
           found == -1 ? "NOT FOUND" : "FOUND", found);
}

static void demo_custom_event_tracking(DebuggerContext* ctx)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||            CUSTOM EVENT TRACKING DEMO                 ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    debugger_reset_counters(ctx);

    debugger_hook(ctx, "Application started");
    debugger_hook_ex(ctx, EVT_INSERT, 1, 100, "Inserted node with value %d", 100);
    debugger_hook_ex(ctx, EVT_INSERT, 2, 200, "Inserted node with value %d", 200);
    debugger_hook_ex(ctx, EVT_UPDATE, 1, 150, "Updated node 1: %d -> %d", 100, 150);
    debugger_hook_ex(ctx, EVT_DELETE, 2, 0, "Deleted node 2");
    debugger_hook_ex(ctx, EVT_CUSTOM, 0, 0, "Performed backup operation");
    debugger_hook(ctx, "Application finished");

    debugger_print_history(ctx);

    printf("  Total steps recorded: %u\n\n", debugger_get_step_count(ctx));
}

static void print_debugger_explanation(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         WHAT IS A STEPPER DEBUGGER?                    ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  A step-debugger allows you to pause algorithm execution\n");
    printf("  and step through it one operation at a time.\n");
    printf("\n");
    printf("  Key Features:\n");
    printf("  -------------\n");
    printf("  - Step-through mode: Execute one operation at a time\n");
    printf("  - Pause/Resume: Stop and continue execution\n");
    printf("  - Event logging: Record all operations performed\n");
    printf("  - Breakpoints: Pause at specific step numbers\n");
    printf("  - History: View recent operations in a scrolling log\n");
    printf("\n");
    printf("  Event Types:\n");
    printf("  -----------\n");
    printf("  - COMPARE: Comparing two values\n");
    printf("  - SWAP: Swapping two elements\n");
    printf("  - VISIT: Visiting a node/element\n");
    printf("  - ENQUEUE/DEQUEUE: Queue operations\n");
    printf("  - RELAX: Edge relaxation (graph algorithms)\n");
    printf("  - INSERT/DELETE: Data structure operations\n");
    printf("\n");
    printf("  Use Cases:\n");
    printf("  ----------\n");
    printf("  - Understanding how algorithms work\n");
    printf("  - Debugging incorrect algorithm behavior\n");
    printf("  - Teaching algorithm concepts\n");
    printf("  - Finding off-by-one errors\n");
    printf("\n");
    printf("  Integration:\n");
    printf("  ------------\n");
    printf("  Call debugger_hook() or debugger_hook_ex() in your\n");
    printf("  algorithm code to record events.\n");
    printf("\n");
}

void debugger_demo(void)
{
    while (1)
    {
        display_header("Interactive Algorithm Step-Debugger & Event Logger");

        printf("\nStep through algorithms one operation at a time.\n");
        printf("Watch events in real-time with a scrolling event log.\n\n");

        printf("1. Bubble Sort with Debugger\n");
        printf("2. Linear Search with Debugger\n");
        printf("3. Custom Event Tracking Demo\n");
        printf("4. Educational Guide\n");
        printf("5. Back to main menu\n");
        printf("\n");

        int choice;
        if (safe_input_int(&choice, "Enter choice: ", 1, 5) != 0)
            break;

        switch (choice)
        {
            case 1:
            {
                DebuggerContext ctx;
                debugger_init(&ctx);
                debugger_enable_step_mode(&ctx);

                printf("\n  Step mode enabled! The debugger will pause at each event.\n");
                printf("  Press [Q] to quit step mode and run at full speed.\n\n");

                demo_bubble_sort_with_debugger(&ctx);

                debugger_destroy(&ctx);
                break;
            }
            case 2:
            {
                DebuggerContext ctx;
                debugger_init(&ctx);
                debugger_enable_step_mode(&ctx);

                printf("\n  Step mode enabled!\n\n");

                demo_linear_search_with_debugger(&ctx);

                debugger_destroy(&ctx);
                break;
            }
            case 3:
            {
                DebuggerContext ctx;
                debugger_init(&ctx);

                demo_custom_event_tracking(&ctx);

                debugger_destroy(&ctx);
                break;
            }
            case 4:
                print_debugger_explanation();
                break;
            case 5:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}