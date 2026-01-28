#include <stdio.h>
#include "stack.h"
#include "safe_input.h"

void dfs(int size,int adj[size][size],int start);

void dfs_demo(void){
    while (1){
        int graph_capacity;
        int graph_capacity_status=safe_input_int(&graph_capacity,
        "\nenter the number of nodes in the graph, (between 1 and 10), enter '-1' to exit : ",
        1,10);

        if(graph_capacity_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting dfs demo.....\n");
            return;
        }

        if(graph_capacity_status==0){
            continue;
        }

        int adj_matrix[graph_capacity][graph_capacity];

        printf("\nenter values in the adjacency matrix: \n");

        for(int i=0;i<graph_capacity;i++){          //nested for loop to take adjacency matrix at runtime
            for(int j=0;j<graph_capacity;j++){
                dfs_node_start:
                printf("\nenter the value of %d row of %d column, (either 1 or 0), enter '-1' to exit : ",i,j);
                int bfs_node_status=safe_input_int(&adj_matrix[i][j],NULL,0,1);
                if(bfs_node_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting dfs demo\n");
                    return;
                }
                if(bfs_node_status==0){
                    goto dfs_node_start;
                }
            }
        }

starting_node_input:
        printf("\nenter starting node, (between 0 to graph_capacity-1) : ");
        int starting_node;
        int starting_node_status=safe_input_int(&starting_node,NULL,0,graph_capacity-1);

        if(starting_node_status==INPUT_EXIT_SIGNAL){
            printf("\nexiting dfs demo...\n");
            return;
        }

        if(starting_node_status==0){
            goto starting_node_input;
        }

        dfs(graph_capacity,adj_matrix,starting_node);
    }
    
}
void dfs(int size,int adj[size][size],int start){
    int visited[size];
    for(int i=0;i<size;i++){        //initializing array with all zeros
        visited[i]=0;
    }
    if(start<0 || start>=size){
        printf("\ninvalid node passed as starting node");
        return;
    }
    
    stack* nodes=createStack();

    if(nodes==NULL){            
        printf("stack could not be initialized due to a malloc failure");
        return;
    }
    visited[start]=1;
    push(nodes,start);
    while(1){                                   //main loop which performs dfs
        int curr=pop(nodes);
        if(curr==-1)    break;
        printf("%d->",curr);
        for(int i=0;i<size;i++){
            if(adj[curr][i] && !visited[i]){
                visited[i]=1;
                push(nodes,i);
            }
        }
    }
    printf("end\n");
    destroyStack(nodes);
    return;
}