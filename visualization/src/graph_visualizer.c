#include "../include/graph_visualizer.h"
#include <math.h>

GraphVisualizer* graph_visualizer_create(int vertices) {
    GraphVisualizer *gv = calloc(1, sizeof(GraphVisualizer));
    if (!gv) return NULL;
    
    gv->num_vertices = vertices;
    gv->visited = calloc(vertices, sizeof(bool));
    gv->parent = malloc(vertices * sizeof(int));
    gv->distance = malloc(vertices * sizeof(int));
    gv->vertex_x = malloc(vertices * sizeof(int));
    gv->vertex_y = malloc(vertices * sizeof(int));
    gv->vertex_labels = malloc(vertices * sizeof(char*));
    
    // Circular layout
    for (int i = 0; i < vertices; i++) {
        double angle = (2 * M_PI * i) / vertices;
        gv->vertex_x[i] = 30 + (int)(20 * cos(angle));
        gv->vertex_y[i] = 10 + (int)(8 * sin(angle));
        gv->vertex_labels[i] = malloc(10);
        sprintf(gv->vertex_labels[i], "v%d", i);
    }
    
    gv->animation_delay_ms = 300;
    return gv;
}

void graph_visualizer_free(GraphVisualizer *gv) {
    if (!gv) return;
    
    free(gv->visited);
    free(gv->parent);
    free(gv->distance);
    free(gv->vertex_x);
    free(gv->vertex_y);
    
    for (int i = 0; i < gv->num_vertices; i++) {
        free(gv->vertex_labels[i]);
    }
    free(gv->vertex_labels);
    
    if (gv->adj_matrix) {
        for (int i = 0; i < gv->num_vertices; i++) {
            free(gv->adj_matrix[i]);
        }
        free(gv->adj_matrix);
    }
    
    free(gv);
}

void graph_visualizer_set_adj_matrix(GraphVisualizer *gv, int **matrix, int size) {
    if (!gv || !matrix || size != gv->num_vertices) return;
    
    gv->adj_matrix = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        gv->adj_matrix[i] = malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            gv->adj_matrix[i][j] = matrix[i][j];
        }
    }
}

void graph_visualizer_draw(GraphVisualizer *gv) {
    if (!gv) return;
    
    visualizer_clear();
    visualizer_draw_header("Graph Visualization");
    
    // Draw edges
    printf("\n");
    for (int i = 0; i < gv->num_vertices; i++) {
        for (int j = i + 1; j < gv->num_vertices; j++) {
            if (gv->adj_matrix && gv->adj_matrix[i][j]) {
                printf("  %s -- %s\n", gv->vertex_labels[i], gv->vertex_labels[j]);
            }
        }
    }
    
    // Draw vertices with colors
    printf("\n  Vertices:\n");
    for (int i = 0; i < gv->num_vertices; i++) {
        const char *color = COLOR_WHITE;
        if (gv->visited && gv->visited[i]) {
            color = BRIGHT_GREEN;
        }
        printf("  %s[%s]%s", color, gv->vertex_labels[i], ANSI_RESET);
        if (gv->distance && gv->distance[i] < 9999) {
            printf(" (dist: %d)", gv->distance[i]);
        }
        printf("\n");
    }
    
    visualizer_draw_footer("Press Ctrl+C to exit");
}

void graph_visualizer_highlight_vertex(GraphVisualizer *gv, int vertex, const char *color) {
    if (!gv || vertex < 0 || vertex >= gv->num_vertices) return;
    printf("\n%s▶ Visiting vertex %s %s %s\n", color, 
           BRIGHT_YELLOW, gv->vertex_labels[vertex], ANSI_RESET);
}

void graph_visualizer_highlight_edge(GraphVisualizer *gv, int from, int to, const char *color) {
    if (!gv || from < 0 || from >= gv->num_vertices || to < 0 || to >= gv->num_vertices) return;
    printf("%s  Edge: %s -> %s %s\n", color, 
           gv->vertex_labels[from], gv->vertex_labels[to], ANSI_RESET);
}

void graph_visualizer_clear_highlights(GraphVisualizer *gv) {
    // Reset visited states
    if (gv && gv->visited) {
        memset(gv->visited, 0, gv->num_vertices * sizeof(bool));
    }
}

void graph_visualizer_animate_bfs(GraphVisualizer *gv, int start) {
    if (!gv || start < 0 || start >= gv->num_vertices) return;
    
    printf("\n" BRIGHT_CYAN "=== BFS Traversal Animation ===\n" ANSI_RESET);
    printf("Start vertex: %s\n\n", gv->vertex_labels[start]);
    
    memset(gv->visited, 0, sizeof(bool) * gv->num_vertices);
    int *queue = malloc(gv->num_vertices * sizeof(int));
    int front = 0, rear = 0;
    int step = 0;
    
    queue[rear++] = start;
    gv->visited[start] = true;
    
    while (front < rear) {
        int current = queue[front++];
        
        graph_visualizer_highlight_vertex(gv, current, BRIGHT_RED);
        graph_visualizer_draw(gv);
        printf(BRIGHT_YELLOW "Step %d: Visiting %s\n" ANSI_RESET, step++, gv->vertex_labels[current]);
        visualizer_delay(gv->animation_delay_ms);
        
        for (int i = 0; i < gv->num_vertices; i++) {
            if (gv->adj_matrix && gv->adj_matrix[current][i] && !gv->visited[i]) {
                gv->visited[i] = true;
                queue[rear++] = i;
                
                graph_visualizer_highlight_edge(gv, current, i, BRIGHT_GREEN);
                printf(BRIGHT_GREEN "  → Found neighbor: %s\n" ANSI_RESET, gv->vertex_labels[i]);
                visualizer_delay(gv->animation_delay_ms / 2);
            }
        }
        
        graph_visualizer_clear_highlights(gv);
        visualizer_delay(gv->animation_delay_ms / 2);
    }
    
    free(queue);
    printf("\n" BRIGHT_GREEN "✅ BFS traversal complete!\n" ANSI_RESET);
}

void graph_visualizer_animate_dfs(GraphVisualizer *gv, int start) {
    if (!gv || start < 0 || start >= gv->num_vertices) return;
    
    printf("\n" BRIGHT_CYAN "=== DFS Traversal Animation ===\n" ANSI_RESET);
    printf("Start vertex: %s\n\n", gv->vertex_labels[start]);
    
    memset(gv->visited, 0, sizeof(bool) * gv->num_vertices);
    int *stack = malloc(gv->num_vertices * sizeof(int));
    int top = -1;
    int step = 0;
    
    stack[++top] = start;
    
    while (top >= 0) {
        int current = stack[top--];
        
        if (gv->visited[current]) continue;
        gv->visited[current] = true;
        
        graph_visualizer_highlight_vertex(gv, current, BRIGHT_MAGENTA);
        graph_visualizer_draw(gv);
        printf(BRIGHT_YELLOW "Step %d: Visiting %s\n" ANSI_RESET, step++, gv->vertex_labels[current]);
        visualizer_delay(gv->animation_delay_ms);
        
        for (int i = gv->num_vertices - 1; i >= 0; i--) {
            if (gv->adj_matrix && gv->adj_matrix[current][i] && !gv->visited[i]) {
                stack[++top] = i;
                graph_visualizer_highlight_edge(gv, current, i, BRIGHT_CYAN);
                visualizer_delay(gv->animation_delay_ms / 2);
            }
        }
    }
    
    free(stack);
    printf("\n" BRIGHT_GREEN "✅ DFS traversal complete!\n" ANSI_RESET);
}

void graph_visualizer_animate_dijkstra(GraphVisualizer *gv, int start) {
    if (!gv || start < 0 || start >= gv->num_vertices) return;
    
    printf("\n" BRIGHT_CYAN "=== Dijkstra's Algorithm Animation ===\n" ANSI_RESET);
    printf("Start vertex: %s\n\n", gv->vertex_labels[start]);
    
    int *dist = malloc(gv->num_vertices * sizeof(int));
    bool *visited = calloc(gv->num_vertices, sizeof(bool));
    
    for (int i = 0; i < gv->num_vertices; i++) {
        dist[i] = 9999;
    }
    dist[start] = 0;
    
    for (int count = 0; count < gv->num_vertices - 1; count++) {
        int min = 9999, u = -1;
        for (int i = 0; i < gv->num_vertices; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        graph_visualizer_highlight_vertex(gv, u, BRIGHT_YELLOW);
        gv->distance = dist;
        graph_visualizer_draw(gv);
        printf(BRIGHT_YELLOW "Selected vertex %s (dist: %d)\n" ANSI_RESET, 
               gv->vertex_labels[u], dist[u]);
        visualizer_delay(gv->animation_delay_ms);
        
        for (int v = 0; v < gv->num_vertices; v++) {
            if (gv->adj_matrix && gv->adj_matrix[u][v] && !visited[v] && 
                dist[u] + gv->adj_matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + gv->adj_matrix[u][v];
                graph_visualizer_highlight_edge(gv, u, v, BRIGHT_GREEN);
                printf(BRIGHT_GREEN "  Updated distance to %s: %d\n" ANSI_RESET, 
                       gv->vertex_labels[v], dist[v]);
                visualizer_delay(gv->animation_delay_ms / 2);
            }
        }
    }
    
    gv->distance = dist;
    graph_visualizer_draw(gv);
    printf("\n" BRIGHT_GREEN "✅ Dijkstra's algorithm complete!\n" ANSI_RESET);
    
    free(dist);
    free(visited);
}

void graph_visualizer_set_speed(GraphVisualizer *gv, int delay_ms) {
    if (gv) {
        gv->animation_delay_ms = delay_ms;
    }
}