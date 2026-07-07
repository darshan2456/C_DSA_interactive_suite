#ifndef STEP_DEBUGGER_H
#define STEP_DEBUGGER_H

#include <stddef.h>

void set_step_mode(int active);
int get_step_mode(void);

void set_paused(int paused);
int get_paused(void);

void algorithm_step_hook(const char* event_msg);

void get_recent_events(char events[5][128], int* count);
void clear_recent_events(void);
void print_recent_events_card(void);

void debugger_demo(void);

#endif // STEP_DEBUGGER_H
