#ifndef CONFIG_H
#define CONFIG_H

// Triggers the interactive TUI/CLI menu for changing speeds
void settings_menu_demo(void);

// Updates the global speed setting internally
void set_animation_speed(int choice);

// The function every algorithm will call instead of sleep_seconds()
void dynamic_sleep(void);

// Helper to print current status
void print_current_speed(void);

// Returns 1 if speed is instant, 0 otherwise
int is_instant(void);

// Checks if the output is a terminal
int is_terminal_interactive(void);

void init_windows_console(void);

// Getters/setters for telemetry tracing option
void set_telemetry_trace_enabled(int enabled);
int is_telemetry_trace_enabled(void);

#endif