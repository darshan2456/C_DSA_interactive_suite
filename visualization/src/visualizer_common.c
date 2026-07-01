#include "../include/visualizer_common.h"

static VisualizerConfig g_config;

void visualizer_init(VisualizerConfig *config) {
    if (config) {
        g_config = *config;
    } else {
        g_config.animation_delay_ms = 200;
        g_config.use_colors = true;
        g_config.show_step_numbers = true;
        g_config.highlight_path = true;
        g_config.terminal_width = 80;
        g_config.terminal_height = 24;
    }
    
    printf(HIDE_CURSOR);
    visualizer_clear();
}

void visualizer_cleanup(void) {
    printf(SHOW_CURSOR);
    fflush(stdout);
}

void visualizer_clear(void) {
    printf(CLEAR_SCREEN);
    printf(MOVE_HOME);
    fflush(stdout);
}

void visualizer_draw_header(const char *title) {
    printf("\n" BRIGHT_CYAN "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║  %-60s ║\n", title);
    printf("╚══════════════════════════════════════════════════════════════╝\n" ANSI_RESET);
    printf("\n");
}

void visualizer_draw_footer(const char *status) {
    printf("\n" BRIGHT_YELLOW "────────────────────────────────────────────────────────────────────\n");
    printf("%s\n" ANSI_RESET, status);
}

void visualizer_delay(int ms) {
    usleep(ms * 1000);
}

int visualizer_get_terminal_width(void) {
    return 80; // Default, can be enhanced with ioctl
}

int visualizer_get_terminal_height(void) {
    return 24; // Default, can be enhanced with ioctl
}