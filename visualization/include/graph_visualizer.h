#ifndef GRAPH_VISUALIZER_H
#define GRAPH_VISUALIZER_H

#include "visualizer_common.h"

typedef struct {
    int **adj_matrix;
    int num_vertices;
    bool *visited;
    int *parent;
    int *distance;
    int *vertex_x;
    int *vertex_y;
    char **vertex_labels;
    int animation_delay_ms;
} GraphVisualizer;

// Creation/Destruction
GraphVisualizer* graph_visualizer_create(int vertices);
void graph_visualizer_free(GraphVisualizer *gv);
void graph_visualizer_set_adj_matrix(GraphVisualizer *gv, int **matrix, int size);

// Rendering
void graph_visualizer_draw(GraphVisualizer *gv);
void graph_visualizer_draw_ascii(GraphVisualizer *gv);

// Highlighting
void graph_visualizer_highlight_vertex(GraphVisualizer *gv, int vertex, const char *color);
void graph_visualizer_highlight_edge(GraphVisualizer *gv, int from, int to, const char *color);
void graph_visualizer_clear_highlights(GraphVisualizer *gv);

// Algorithm animations
void graph_visualizer_animate_bfs(GraphVisualizer *gv, int start);
void graph_visualizer_animate_dfs(GraphVisualizer *gv, int start);
void graph_visualizer_animate_dijkstra(GraphVisualizer *gv, int start);

// Utility
void graph_visualizer_set_speed(GraphVisualizer *gv, int delay_ms);

#endif