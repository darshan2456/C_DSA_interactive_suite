#ifndef VISUALIZER_COMMON_H
#define VISUALIZER_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// ANSI Color Codes
#define ANSI_RESET      "\033[0m"
#define ANSI_BOLD       "\033[1m"
#define ANSI_DIM        "\033[2m"
#define ANSI_UNDERLINE  "\033[4m"

// Foreground Colors
#define COLOR_BLACK     "\033[30m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"

// Bright Colors
#define BRIGHT_RED      "\033[91m"
#define BRIGHT_GREEN    "\033[92m"
#define BRIGHT_YELLOW   "\033[93m"
#define BRIGHT_BLUE     "\033[94m"
#define BRIGHT_MAGENTA  "\033[95m"
#define BRIGHT_CYAN     "\033[96m"

// Background Colors
#define BG_RED          "\033[41m"
#define BG_GREEN        "\033[42m"
#define BG_YELLOW       "\033[43m"
#define BG_BLUE         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"

// Screen Control
#define CLEAR_SCREEN    "\033[2J"
#define CLEAR_LINE      "\033[K"
#define MOVE_HOME       "\033[H"
#define HIDE_CURSOR     "\033[?25l"
#define SHOW_CURSOR     "\033[?25h"
#define SAVE_CURSOR     "\033[s"
#define RESTORE_CURSOR  "\033[u"

// Box Drawing (Unicode)
#define BOX_TL          "┌"
#define BOX_TR          "┐"
#define BOX_BL          "└"
#define BOX_BR          "┘"
#define BOX_H           "─"
#define BOX_V           "│"

typedef struct {
    int animation_delay_ms;
    bool use_colors;
    bool show_step_numbers;
    bool highlight_path;
    int terminal_width;
    int terminal_height;
} VisualizerConfig;

// Common functions
void visualizer_init(VisualizerConfig *config);
void visualizer_cleanup(void);
void visualizer_clear(void);
void visualizer_draw_header(const char *title);
void visualizer_draw_footer(const char *status);
void visualizer_delay(int ms);
int visualizer_get_terminal_width(void);
int visualizer_get_terminal_height(void);

#endif