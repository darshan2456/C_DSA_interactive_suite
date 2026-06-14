#include "backtracking.h"
#include "cross_platform.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define clear_screen() system("cls")
#else
    #define _DEFAULT_SOURCE
    #include <unistd.h>
    #define clear_screen() printf("\033[H\033[J")
#endif

#define MAX_V 10

typedef struct {
    const char* name;
    int num_vertices;
    int matrix[MAX_V][MAX_V];
} GraphTopology;

static const char* color_names[] = { "NONE", "RED", "GREEN", "BLUE", "YELLOW", "MAGENTA" };
static const char* color_ansi[] = { 
    "\033[90m",          // Gray (Unassigned)
    "\033[38;5;196;1m",   // Bright Red
    "\033[38;5;46;1m",    // Bright Green
    "\033[38;5;21;1m",    // Bright Blue
    "\033[38;5;226;1m",   // Bright Yellow
    "\033[38;5;201;1m"    // Bright Magenta
};

// ANSI reset
#define ANSI_COLOR_RESET "\033[0m"

// Graph Drawing Templates (escaped backslashes for C string literals)
static const char* c5_template =
"             %s\n"
"            /        \\\\\n"
"           /          \\\\\n"
"         %s          %s\n"
"          |            |\n"
"          |            |\n"
"         %s----------%s\n";

static const char* k33_template =
"       %s ------------------ %s\n"
"         \\\\   \\\\    \\\\      /    /   /\n"
"          \\\\   \\\\    \\\\    /    /   /\n"
"           %s -----\\\\--/----- %s\n"
"          /   /    /    \\\\    \\\\   \\\\\n"
"         /   /    /      \\\\    \\\\   \\\\\n"
"       %s ------------------ %s\n";

static const char* k4_template =
"       %s ------------------ %s\n"
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

static const char* k5_template =
"                  %s\n"
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

static const char* w5_template =
"       %s ------------------ %s\n"
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

static const char* petersen_template =
"                    %s\n"
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
    snprintf(buffer, size, "%s%s●(V%d)%s" ANSI_COLOR_RESET, border_start, label_color, v, border_end);
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
        printf(petersen_template, labels[0], labels[5], labels[4], labels[1], labels[9], labels[6], labels[8], labels[7], labels[3], labels[2]);
    }
    printf("\n");
}

static void print_graph_state(const GraphTopology* graph, int colors[MAX_V], int current_vertex, const char* status_msg, int delay_time)
{
    clear_screen();
    printf("\n=== GRAPH COLORING BACKTRACKING VISUALIZER ===\n\n");
    printf("Topology: \033[38;5;208;1m%s\033[0m (%d vertices)\n\n", graph->name, graph->num_vertices);

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

static int solve_graph_coloring_util(const GraphTopology* graph, int m, int colors[MAX_V], int v, int delay_time)
{
    // Base Case: All vertices are colored
    if (v == graph->num_vertices)
    {
        print_graph_state(graph, colors, -1, "🎉 SUCCESS: All vertices colored successfully!", delay_time);
        return 1;
    }

    // Try different colors for vertex v
    for (int c = 1; c <= m; c++)
    {
        char status_msg[128];
        snprintf(status_msg, sizeof(status_msg), "Trying to assign color %s%s" ANSI_COLOR_RESET " to V%d", color_ansi[c], color_names[c], v);
        print_graph_state(graph, colors, v, status_msg, delay_time);

        if (is_safe(v, graph, colors, c))
        {
            colors[v] = c;
            snprintf(status_msg, sizeof(status_msg), "Safe! Assigned color %s%s" ANSI_COLOR_RESET " to V%d. Recursing...", color_ansi[c], color_names[c], v);
            print_graph_state(graph, colors, v, status_msg, delay_time);

            if (solve_graph_coloring_util(graph, m, colors, v + 1, delay_time))
            {
                return 1;
            }

            // Backtrack
            colors[v] = 0;
            snprintf(status_msg, sizeof(status_msg), "Conflict down the line! Backtracking... unassigning V%d", v);
            print_graph_state(graph, colors, v, status_msg, delay_time);
        }
        else
        {
            snprintf(status_msg, sizeof(status_msg), "Conflict! Cannot assign %s%s" ANSI_COLOR_RESET " to V%d (adjacent conflict)", color_ansi[c], color_names[c], v);
            print_graph_state(graph, colors, v, status_msg, delay_time);
        }
    }

    return 0;
}

void graph_coloring_demo(void)
{
    // Set console output to UTF-8 on Windows for Unicode support (●)
#ifdef _WIN32
    #include <windows.h>
    UINT old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Predefined Graph Topologies
    GraphTopology topologies[] = {
        {
            "Cycle Graph C5", 5,
            {
                {0, 1, 0, 0, 1},
                {1, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 1},
                {1, 0, 0, 1, 0}
            }
        },
        {
            "Bipartite Graph K3,3", 6,
            {
                {0, 0, 0, 1, 1, 1},
                {0, 0, 0, 1, 1, 1},
                {0, 0, 0, 1, 1, 1},
                {1, 1, 1, 0, 0, 0},
                {1, 1, 1, 0, 0, 0},
                {1, 1, 1, 0, 0, 0}
            }
        },
        {
            "Complete Graph K4", 4,
            {
                {0, 1, 1, 1},
                {1, 0, 1, 1},
                {1, 1, 0, 1},
                {1, 1, 1, 0}
            }
        },
        {
            "Complete Graph K5", 5,
            {
                {0, 1, 1, 1, 1},
                {1, 0, 1, 1, 1},
                {1, 1, 0, 1, 1},
                {1, 1, 1, 0, 1},
                {1, 1, 1, 1, 0}
            }
        },
        {
            "Wheel Graph W5", 5,
            {
                {0, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 0, 1, 0},
                {1, 0, 1, 0, 1},
                {1, 1, 0, 1, 0}
            }
        },
        {
            "Petersen Graph", 10,
            {
                {0, 1, 0, 0, 1, 1, 0, 0, 0, 0},
                {1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
                {0, 1, 0, 1, 0, 0, 0, 1, 0, 0},
                {0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
                {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
                {0, 1, 0, 0, 0, 0, 0, 0, 1, 1},
                {0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
                {0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 1, 1, 0, 0}
            }
        }
    };
    int num_topologies = sizeof(topologies) / sizeof(topologies[0]);

    while (1)
    {
        int graph_choice;
        printf("\n--- Graph Coloring Topology ---\n");
        for (int i = 0; i < num_topologies; i++)
        {
            printf("%d. %s (%d vertices)\n", i + 1, topologies[i].name, topologies[i].num_vertices);
        }
        int status = safe_input_int(&graph_choice, "Select topology (1-6), or -1 to exit: ", 1, num_topologies);

        if (status == INPUT_EXIT_SIGNAL)
        {
#ifdef _WIN32
            SetConsoleOutputCP(old_cp); // restore code page
#endif
            printf("\nReturning to Backtracking menu...\n");
            return;
        }

        if (status == 0)
            continue;

        const GraphTopology* selected_graph = &topologies[graph_choice - 1];

        int M;
        status = safe_input_int(&M, "\nEnter the number of colors M (between 1 and 5), or -1 to exit: ", 1, 5);
        if (status == INPUT_EXIT_SIGNAL)
        {
#ifdef _WIN32
            SetConsoleOutputCP(old_cp);
#endif
            printf("\nReturning to Backtracking menu...\n");
            return;
        }
        if (status == 0)
            continue;

        int delay_time = 150;
        int colors[MAX_V] = {0};

        print_graph_state(selected_graph, colors, 0, "Starting graph coloring...", delay_time);

        clock_t start_t = clock();
        int solved = solve_graph_coloring_util(selected_graph, M, colors, 0, delay_time);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        if (solved)
        {
            printf("\n🎉 SUCCESS: The graph was successfully colored using %d colors!\n", M);
        }
        else
        {
            printf("\n❌ FAILURE: No valid coloring exists for this graph with %d colors.\n", M);
        }
        printf("Time taken: %f seconds\n", total_t);
    }
}
