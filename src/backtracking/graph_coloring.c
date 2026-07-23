#include "../utils/config.h"
#include "backtracking.h"
#include "cross_platform_timer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "clear_screen.h"

static const char* color_names[] = {"NONE", "RED", "GREEN", "BLUE", "YELLOW", "MAGENTA"};
static const char* color_ansi[] = {
    "\033[90m",         // Gray (Unassigned)
    "\033[38;5;196;1m", // Bright Red
    "\033[38;5;46;1m",  // Bright Green
    "\033[38;5;21;1m",  // Bright Blue
    "\033[38;5;226;1m", // Bright Yellow
    "\033[38;5;201;1m"  // Bright Magenta
};

// ANSI reset
#define ANSI_COLOR_RESET "\033[0m"

// Graph Drawing Templates (escaped backslashes for C string literals)
static const char* c5_template = "             %s\n"
                                 "            /        \\\\\n"
                                 "           /          \\\\\n"
                                 "         %s          %s\n"
                                 "          |            |\n"
                                 "          |            |\n"
                                 "         %s----------%s\n";

static const char* k33_template = "       %s ------------------ %s\n"
                                  "         \\\\   \\\\    \\\\      /    /   /\n"
                                  "          \\\\   \\\\    \\\\    /    /   /\n"
                                  "           %s -----\\\\--/----- %s\n"
                                  "          /   /    /    \\\\    \\\\   \\\\\n"
                                  "         /   /    /      \\\\    \\\\   \\\\\n"
                                  "       %s ------------------ %s\n";

static const char* k4_template = "       %s ------------------ %s\n"
                                 "        |   \\\\              /   |\n"
                                 "        |    \\\\            /    |\n"
                                 "        |     \\\\          /     |\n"
                                 "        |      \\\\        /      |\n"
                                 "        |       \\\\      /       |\n"
                                 "        |        \\\\    /        |\n"
                                 "        |         \\\\  /         |\n"
                                 "        |          \\\\/          |\n"
                                 "        |          /\\\\          |\n"
                                 "        |         /  \\\\         |\n"
                                 "        |        /    \\\\        |\n"
                                 "        |       /      \\\\       |\n"
                                 "        |      /        \\\\      |\n"
                                 "        |     /          \\\\     |\n"
                                 "        |    /            \\\\    |\n"
                                 "        |   /              \\\\   |\n"
                                 "       %s ------------------ %s\n";

static const char* k5_template = "                  %s\n"
                                 "                / /  \\\\ \\\n"
                                 "              /  /    \\\\  \\\n"
                                 "            /   /      \\\\   \\\n"
                                 "         %s -------------- %s\n"
                                 "          | \\\\ \\\\          / / |\n"
                                 "          |  \\\\  \\\\      /  /  |\n"
                                 "          |   \\\\   \\\\  /   /   |\n"
                                 "          |     \\\\  X  /      |\n"
                                 "          |    /  /  \\\\  \\\\    |\n"
                                 "          |  /  /      \\\\  \\\\  |\n"
                                 "          | / /          \\\\ \\\\ |\n"
                                 "         %s -------------- %s\n";

static const char* w5_template = "       %s ------------------ %s\n"
                                 "        |   \\\\              /   |\n"
                                 "        |    \\\\            /    |\n"
                                 "        |     \\\\          /     |\n"
                                 "        |      \\\\        /      |\n"
                                 "        |       \\\\      /       |\n"
                                 "        |        \\\\    /        |\n"
                                 "        |         %s           |\n"
                                 "        |        /    \\\\        |\n"
                                 "        |       /      \\\\       |\n"
                                 "        |      /        \\\\      |\n"
                                 "        |     /          \\\\     |\n"
                                 "        |    /            \\\\    |\n"
                                 "        |   /              \\\\   |\n"
                                 "       %s ------------------ %s\n";

static const char* petersen_template = "                    %s\n"
                                       "                   / | \\\\\n"
                                       "                  /  |  \\\\\n"
                                       "                /   %s   \\\\\n"
                                       "              /    /   \\\\   \\\\\n"
                                       "           %s    /       \\\\    %s\n"
                                       "            |  %s ------- %s  |\n"
                                       "            |   \\\\  \\\\     /  /   |\n"
                                       "            |    \\\\   \\\\ /   /    |\n"
                                       "            |     \\\\   X   /     |\n"
                                       "            |    /   / \\\\   \\\\    |\n"
                                       "            |   /  /     \\\\  \\\\   |\n"
                                       "            |  %s ------- %s  |\n"
                                       "           %s                %s\n"
                                       "             \\\\              /\n"
                                       "               \\\\----------/\n";

static void get_vertex_label(int v, int color_code, char* buffer, size_t size, int is_current)
{
    const char* label_color = color_ansi[color_code];
    const char* border_start = is_current ? "\033[38;5;226;1m[\033[0m" : " ";
    const char* border_end = is_current ? "\033[38;5;226;1m]\033[0m" : " ";
    snprintf(buffer, size, "%s%s●(V%d)%s" ANSI_COLOR_RESET, border_start, label_color, v,
             border_end);
}

static void print_drawn_graph(const GraphTopology* graph, int colors[MAX_V], int current_vertex)
{
    char labels[MAX_V][64];
    for (int i = 0; i < graph->num_vertices; i++)
    {
        get_vertex_label(i, colors[i], labels[i], sizeof(labels[i]), (i == current_vertex));
    }

    printf("Drawn Graph Structure:\n");
    if (strcmp(graph->name, "Cycle Graph C5") == 0)
    {
        printf(c5_template, labels[0], labels[4], labels[1], labels[3], labels[2]);
    }
    else if (strcmp(graph->name, "Bipartite Graph K3,3") == 0)
    {
        printf(k33_template, labels[0], labels[3], labels[1], labels[4], labels[2], labels[5]);
    }
    else if (strcmp(graph->name, "Complete Graph K4") == 0)
    {
        printf(k4_template, labels[0], labels[1], labels[3], labels[2]);
    }
    else if (strcmp(graph->name, "Complete Graph K5") == 0)
    {
        printf(k5_template, labels[0], labels[4], labels[1], labels[3], labels[2]);
    }
    else if (strcmp(graph->name, "Wheel Graph W5") == 0)
    {
        printf(w5_template, labels[1], labels[2], labels[0], labels[4], labels[3]);
    }
    else if (strcmp(graph->name, "Petersen Graph") == 0)
    {
        printf(petersen_template, labels[0], labels[5], labels[4], labels[1], labels[9], labels[6],
               labels[8], labels[7], labels[3], labels[2]);
    }
    printf("\n");
}

void print_graph_state(const GraphTopology* graph, int colors[MAX_V], int current_vertex,
                       const char* status_msg, int delay_time)
{
    if (is_instant())
    {
        return;
    }
    clear_screen();
    printf("\n=== GRAPH COLORING BACKTRACKING VISUALIZER ===\n\n");
    printf("Topology: \033[38;5;208;1m%s\033[0m (%d vertices)\n\n", graph->name,
           graph->num_vertices);

    // Print Adjacency Matrix representation
    printf("Adjacency Matrix:\n");
    printf("     ");
    for (int i = 0; i < graph->num_vertices; i++)
    {
        printf(" V%d", i);
    }
    printf("\n");
    for (int i = 0; i < graph->num_vertices; i++)
    {
        printf(" V%d: [", i);
        for (int j = 0; j < graph->num_vertices; j++)
        {
            if (graph->matrix[i][j])
            {
                printf("\033[32m 1\033[0m");
            }
            else
            {
                printf(" 0");
            }
        }
        printf(" ]\n");
    }
    printf("\n");

    // Print connectivity & current state of color assignments
    printf("Vertex States & Connections:\n");
    for (int i = 0; i < graph->num_vertices; i++)
    {
        const char* arrow = (i == current_vertex) ? " \033[38;5;226;1m<--- Coloring\033[0m" : "";
        printf("  Vertex V%d: ", i);
        printf("%s● %-10s" ANSI_COLOR_RESET, color_ansi[colors[i]], color_names[colors[i]]);
        printf(" | Connected to:");
        for (int j = 0; j < graph->num_vertices; j++)
        {
            if (graph->matrix[i][j])
            {
                printf(" V%d", j);
            }
        }
        printf("%s\n", arrow);
    }
    printf("\n");

    // Print ASCII/Unicode structural visualization of the graph
    print_drawn_graph(graph, colors, current_vertex);

    printf("\nStatus: \033[38;5;51;1m%s\033[0m\n", status_msg);
    printf("Delay: %dms\n", delay_time);
    fflush(stdout);
    sleep_seconds((float)delay_time / 1000);
}

static int is_safe(int v, const GraphTopology* graph, int colors[MAX_V], int c)
{
    for (int i = 0; i < graph->num_vertices; i++)
    {
        if (graph->matrix[v][i] == 1 && colors[i] == c)
        {
            return 0;
        }
    }
    return 1;
}

int solve_graph_coloring_util(const GraphTopology* graph, int m, int colors[MAX_V], int v,
                              int delay_time)
{
    // If all vertices are assigned colors, return true
    if (v == graph->num_vertices)
    {
        return 1;
    }

    // Try different colors for vertex v
    for (int c = 1; c <= m; c++)
    {
        // Check if assignment of color c to v is fine
        if (is_safe(v, graph, colors, c))
        {
            colors[v] = c;
            char msg[128];
            snprintf(msg, sizeof(msg), "Assigned %s to Vertex V%d. Checking neighbors...",
                     color_names[c], v);
            print_graph_state(graph, colors, v, msg, delay_time);

            // Recur to assign colors to rest of the vertices
            if (solve_graph_coloring_util(graph, m, colors, v + 1, delay_time))
            {
                return 1;
            }

            // If assigning color c doesn't lead to a solution then remove it
            colors[v] = 0;
            snprintf(msg, sizeof(msg), "Conflict detected! Backtracking color of Vertex V%d...", v);
            print_graph_state(graph, colors, v, msg, delay_time);
        }
    }

    return 0;
}

// --- TEST WRAPPER ---
bool run_graph_coloring_test(int colors_allowed)
{
    // Inject a K4 graph directly to bypass the menu selection
    GraphTopology k4 = {"Test K4", 4, {{0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1}, {1, 1, 1, 0}}};
    int colors[MAX_V] = {0};
    return solve_graph_coloring_util(&k4, colors_allowed, colors, 0, 0) == 1;
}