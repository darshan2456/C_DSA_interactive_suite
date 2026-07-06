#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stddef.h>

#define MAX_EVENT_HISTORY 20
#define MAX_EVENT_MSG_LEN 128

typedef enum {
    DEBUG_STEP,
    DEBUG_PAUSE,
    DEBUG_RUN
} DebugState;

typedef enum {
    EVT_COMPARE,
    EVT_SWAP,
    EVT_INSERT,
    EVT_DELETE,
    EVT_VISIT,
    EVT_ENQUEUE,
    EVT_DEQUEUE,
    EVT_RELAX,
    EVT_UPDATE,
    EVT_MARK,
    EVT_CUSTOM
} EventType;

typedef struct {
    EventType type;
    int data1;
    int data2;
    char message[MAX_EVENT_MSG_LEN];
    unsigned int step_number;
} DebugEvent;

typedef struct {
    DebugState state;
    int step_mode;
    int auto_continue;
    unsigned int total_steps;
    unsigned int breakpoints[MAX_EVENT_HISTORY];
    int num_breakpoints;
    DebugEvent event_history[MAX_EVENT_HISTORY];
    int event_count;
    unsigned int start_step;
    unsigned int target_step;
} DebuggerContext;

void debugger_init(DebuggerContext* ctx);
void debugger_destroy(DebuggerContext* ctx);

void debugger_set_mode(DebuggerContext* ctx, DebugState state);
DebugState debugger_get_mode(DebuggerContext* ctx);

void debugger_enable_step_mode(DebuggerContext* ctx);
void debugger_disable_step_mode(DebuggerContext* ctx);
int debugger_is_step_mode(DebuggerContext* ctx);

void debugger_pause(DebuggerContext* ctx);
void debugger_resume(DebuggerContext* ctx);
int debugger_is_paused(DebuggerContext* ctx);

void debugger_step(DebuggerContext* ctx);

void debugger_add_breakpoint(DebuggerContext* ctx, unsigned int step);
void debugger_remove_breakpoint(DebuggerContext* ctx, unsigned int step);
void debugger_clear_breakpoints(DebuggerContext* ctx);
int debugger_has_breakpoint(DebuggerContext* ctx, unsigned int step);

const char* debugger_event_type_str(EventType type);
const char* debugger_state_str(DebugState state);

void debugger_hook(DebuggerContext* ctx, const char* event_msg);
void debugger_hook_ex(DebuggerContext* ctx, EventType type, int data1, int data2, const char* format, ...);

void debugger_print_event(DebugEvent* evt);
void debugger_print_history(DebuggerContext* ctx);
void debugger_print_status(DebuggerContext* ctx);

void debugger_reset_counters(DebuggerContext* ctx);
unsigned int debugger_get_step_count(DebuggerContext* ctx);

int debugger_wait_for_input(DebuggerContext* ctx);

void debugger_demo(void);

#endif // DEBUGGER_H