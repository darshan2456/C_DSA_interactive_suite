#include "advanced_graph_algorithms.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void advanced_graph_algorithms_demo(void)
{
    while (1)
    {
        display_header("Advanced Graph Algorithms");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nAdvanced Graph Algorithms Demo\n"
                                    "------------------------------\n"
                                    "\n"
                                    "1. Strongly Connected Components (SCC)\n"
                                    "2. Maximum Flow\n"
                                    "3. Bipartite Matching\n"
                                    "4. Eulerian Path\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 4);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nReturning to main menu...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Strongly Connected Components (SCC)");
                scc_demo();
                break;
            case 2:
                display_header("Maximum Flow");
                max_flow_demo();
                break;
            case 3:
                display_header("Bipartite Matching");
                bipartite_matching_demo();
                break;
            case 4:
                display_header("Eulerian Path");
                eulerian_path_demo();
                break;
        }
    }
}
