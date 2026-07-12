#ifndef HELP_H
#define HELP_H

/**
 * @brief Displays an interactive, Linux man-page style help guide.
 * Bypasses normal input flow until the user exits the guide.
 */
void launch_help_page(void);

/**
 * @brief Sub-menu for Data Structures help.
 */
void help_data_structures_menu(void);

/**
 * @brief Sub-menu for Sorting & Searching help.
 */
void help_sorting_searching_menu(void);

/**
 * @brief Sub-menu for Graphs & Trees help.
 */
void help_graphs_trees_menu(void);

/**
 * @brief Sub-menu for Advanced Topics help.
 */
void help_advanced_topics_menu(void);

/**
 * @brief Sub-menu for Expression Evaluation help.
 */
void help_expression_evaluation_menu(void);

#endif // HELP_H