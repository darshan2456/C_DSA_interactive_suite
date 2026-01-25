#include<stdio.h>
#include "safe_input.h"
#include "graph_traversals.h"

void graph_traversals_demo(void){
    while(1){
        int graph_traversal_choice;
        int graph_traversal_status=safe_input_int(&graph_traversal_choice,
        "\nenter 1 for bfs and 2 for dfs : ",
        1,2);

        if(graph_traversal_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting graph traversal demo...\n");
            return;
        }

        if(graph_traversal_status==0){
            continue;
        }

        switch(graph_traversal_choice){
            case 1:
                bfs_demo();
                break;
            case 2:
                dfs_demo();
                break;
        }
        
    }
}