#include "step_debugger.h"
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <ncurses.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#else
#include <conio.h>
#endif
static char event_log[5][128];
static int event_count = 0;

#ifndef _WIN32
static int get_keypress_unix(int block)
{
    if (stdscr != NULL && !isendwin())
    {
        nodelay(stdscr, block ? FALSE : TRUE);
        int ch = getch();
        return ch;
    }

    struct termios oldt, newt;
    memset(&oldt, 0, sizeof(oldt));
    memset(&newt, 0, sizeof(newt));
    int ch = -1;
    int is_tty = (tcgetattr(STDIN_FILENO, &oldt) == 0);
    if (is_tty)
    {
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    if (block)
    {
        ch = getchar();
    }
    else
    {
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0)
        {
            ch = getchar();
        }
    }

    if (is_tty)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
    return ch;
}
#else
static int get_keypress_windows(int block)
{
    if (block)
    {
        return _getch();
    }
    else
    {
        if (_kbhit())
        {
            return _getch();
        }
    }
    return -1;
}
#endif

static int get_keypress(int block)
{
#ifdef _WIN32
    return get_keypress_windows(block);
#else
    return get_keypress_unix(block);
#endif
}

void algorithm_step_hook(const char* event_msg)
{
    if (event_msg == NULL)
        return;

    // Shift events in circular log
    if (event_count < 5)
    {
        strncpy(event_log[event_count], event_msg, 127);
        event_log[event_count][127] = '\0';
        event_count++;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            strcpy(event_log[i], event_log[i + 1]);
        }
        strncpy(event_log[4], event_msg, 127);
        event_log[4][127] = '\0';
    }

    if (!get_step_mode())
        return;

    // Non-blocking keypress check to see if user wants to PAUSE during run
    if (!get_paused())
    {
        int ch = get_keypress(0);
        if (ch == ' ' || ch == 32)
        {
            set_paused(1);
            printf("\n[Paused by User]\n");
        }
        else if (ch == 'q' || ch == 'Q')
        {
            set_step_mode(0);
            set_paused(0);
            printf("\n[Step Debugger Disabled]\n");
        }
    }

    print_recent_events_card();

    while (get_step_mode() && get_paused())
    {
        printf("[Paused] Press [Space] to Resume, [s] to Step, [q] to Quit...\n");
        fflush(stdout);

        int ch = get_keypress(1);
        if (ch == ' ' || ch == 32)
        {
            set_paused(0);
            printf("Resuming execution...\n");
        }
        else if (ch == 's' || ch == 'S')
        {
            break; // Single step
        }
        else if (ch == 'q' || ch == 'Q')
        {
            set_step_mode(0);
            set_paused(0);
            printf("Exiting step debugger mode. Running at full speed.\n");
        }
    }
}

void get_recent_events(char events[5][128], int* count)
{
    *count = event_count;
    for (int i = 0; i < event_count; i++)
    {
        strcpy(events[i], event_log[i]);
    }
}

void clear_recent_events(void)
{
    event_count = 0;
}

void print_recent_events_card(void)
{
    printf("\n┌──────────────────────────────────────────────────┐\n");
    printf("│             DEBUGGER EVENT HISTORY               │\n");
    printf("├──────────────────────────────────────────────────┤\n");
    for (int i = 0; i < 5; i++)
    {
        if (i < event_count)
        {
            printf("│ %02d. %-44s │\n", i + 1, event_log[i]);
        }
        else
        {
            printf("│ %02d. %-44s │\n", i + 1, "---");
        }
    }
    printf("└──────────────────────────────────────────────────┘\n");
}
