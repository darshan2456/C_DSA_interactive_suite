
// #include <math.h>
#include <ncurses.h>
// #include <stdlib.h>
#include <string.h>
// #include <locale.h>

#include "tui.h"
#include "data_structures.h"
#include "backtracking.h"
#include "dynamic_programming.h"
#include "sorting_algorithms_n2.h"
#include "searching_algorithms.h"
#include "job_scheduling.h"
#include "hash.h"
#include "graph_traversals.h"
#include "error_correction_algorithms.h"
#include "trees.h"
#include "advanced_sorting.h"
#include "expression.h"
#include "safe_input.h"

/* ── types ──────────────────────────────────────────────────────────────────── */
typedef void (*demo_fn)(void);

// for non void return type demos
static void linear_search_demo_wrapper(void) {(void)linear_search_demo(); }
static void binary_search_demo_wrapper(void) {(void)binary_search_demo(); }

typedef struct
{
    const char* name;
    demo_fn fn;
    int is_folder; /* 1 = category header, 0 = runnable item */
    int expanded;  /* folders only */
    int depth;     /* 0 = root folder, 1 = item */
} Entry;

/* ── registry ───────────────────────────────────────────────────────────────── */
/*
 * Add new demos here — one line per entry.
 * is_folder=1 → category header (fn=NULL, expanded starts at 1)
 * is_folder=0 → runnable item   (fn=pointer to demo)
 */
static Entry ENTRIES[] = {
    /* name                    fn          folder  expanded  depth */
    {"data_structures", NULL, 1, 1, 0},
    {"Linear Data Structures", NULL, 1, 0, 0},
    {"Singly Linked List", sll_Demo, 0, 0, 1},
    {"Doubly Linked List", dll_demo, 0, 0, 1},
    {"Array", array_demo, 0, 0, 1},
    {"Priority Queue", priority_queue_demo, 0, 0, 1},
    {"Linear Queue", simple_queue_Demo, 0, 0, 1},
    {"Circular Data Structures", NULL, 1, 0, 0},
    {"Circular Queue", circular_queue_Demo, 0, 0, 1},
    {"Singly Circular Queue", scll_Demo, 0, 0, 1},
    {"Double-ended Queue", deque_demo, 0, 0, 1},

    
    {"Backtracking", NULL, 1, 1, 0},
    {"N queens", n_queens_demo, 0, 0, 1},
    {"Sudoku", sudoku_demo, 0, 0, 1},
    {"Rat in a Maze", rat_in_maze_demo, 0, 0, 1},
    {"Graph Coloring", graph_coloring_demo, 0, 0, 1},
    {"Knights Tour", knights_tour_demo, 0, 0, 1},
    
    
    
    {"Dynamic Programming", NULL, 1, 1, 0},
    {"Knapsack", knapsack_demo, 0, 0, 1},
    {"lcs", lcs_demo, 0, 0, 1},
    {"fibonacci", fibonacci_demo, 0, 0, 1},
    {"matrix Chain", mcm_demo, 0, 0, 1},


    {"trees", NULL, 1, 1, 0},
    {"Binary Search Tree", binary_search_tree_Demo, 0, 0, 1},
    {"AVL Tree", avl_demo, 0, 0, 1},

    {"sorting_algorithms_n2", NULL, 1, 1, 0},
    {"Bubble Sort", bubble_sort_optimized_demo, 0, 0, 1}, /* add fn when known */
    {"Selection Sort", selection_sort_demo, 0, 0, 1},
    {"Insertion Sort", insertion_sort_demo, 0, 0, 1},

    {"advanced_sorting", NULL, 1, 1, 0},
    {"Quick Sort", quicksort_demo, 0, 0, 1},
    {"Merge Sort", merge_sort_demo, 0, 0, 1},
    {"Heap Sort", heap_sort_demo, 0, 0, 1},
    {"Radix Sort", radix_sort_demo, 0, 0, 1},

    {"searching_algorithms", NULL, 1, 1, 0},
    {"Linear Search", linear_search_demo_wrapper, 0, 0, 1},
    {"Binary Search", binary_search_demo_wrapper, 0, 0, 1},

    {"graph_traversals", NULL, 1, 1, 0},
    {"BFS", bfs_demo, 0, 0, 1},
    {"DFS", dfs_demo, 0, 0, 1},
    {"Dijkstra", dijkstra_demo, 0, 0, 1},

    {"hashing", NULL, 1, 1, 0},
    {"Linear Probing", linear_probing_demo, 0, 0, 1},
    {"Separate Chaining", separate_chaining_demo, 0, 0, 1},
    {"Double Hashing", double_hashing_demo, 0, 0, 1},

    {"expression_evaluation", NULL, 1, 1, 0},
    {"Infix to Postfix", infix_to_postfix_Demo, 0, 0, 1},
    {"Postfix Evaluation", parantheses_checker_demo, 0, 0, 1},
};

static const int ENTRY_COUNT = sizeof(ENTRIES) / sizeof(ENTRIES[0]);

/* ── state ──────────────────────────────────────────────────────────────────── */
typedef enum
{
    PANE_NAV,
    PANE_VIZ
} ActivePane;

typedef struct
{
    int nav_cursor;    /* index into visible list   */
    ActivePane active; /* which pane has focus      */
    char last_ran[64]; /* name of last run demo     */
    int demo_ran;      /* 1 if a demo has been run  */
} State;

/* ── visible list ───────────────────────────────────────────────────────────── */
/* builds a flat visible list respecting folder expanded state */
static int build_visible(int* visible, int max)
{
    int count = 0;
    int folder = -1;

    for (int i = 0; i < ENTRY_COUNT && count < max; i++)
    {
        if (ENTRIES[i].is_folder)
        {
            folder = i;
            visible[count++] = i;
        }
        else
        {
            /* only show if parent folder is expanded */
            if (folder >= 0 && ENTRIES[folder].expanded)
            {
                visible[count++] = i;
            }
        }
    }
    return count;
}

/* find parent folder of entry at index */
static int find_parent(int idx)
{
    for (int i = idx - 1; i >= 0; i--)
    {
        if (ENTRIES[i].is_folder)
            return i;
    }
    return -1;
}

/* ── colors ─────────────────────────────────────────────────────────────────── */
#define COL_TITLE 1
#define COL_FOLDER 2
#define COL_ITEM 3
#define COL_SELECT 4
#define COL_BORDER 5
#define COL_VIZPANE 6
#define COL_STATUS 7

static void init_colors(void)
{
    if (!has_colors())
        return;
    start_color();
    use_default_colors();

    init_pair(COL_TITLE, COLOR_CYAN, -1);
    init_pair(COL_FOLDER, COLOR_YELLOW, -1);
    init_pair(COL_ITEM, COLOR_WHITE, -1);
    init_pair(COL_SELECT, COLOR_BLACK, COLOR_CYAN);
    init_pair(COL_BORDER, COLOR_BLUE, -1);
    init_pair(COL_VIZPANE, COLOR_GREEN, -1);
    init_pair(COL_STATUS, COLOR_BLACK, COLOR_BLUE);
}

/* ── draw helpers ───────────────────────────────────────────────────────────── */
static void draw_border(WINDOW* win, const char* title, int color)
{
    wattron(win, COLOR_PAIR(color) | A_BOLD);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " %s ", title);
    wattroff(win, COLOR_PAIR(color) | A_BOLD);
}

static void draw_nav(WINDOW* nav, int* visible, int vis_count, int cursor, int active)
{
    int rows, cols;
    getmaxyx(nav, rows, cols);
    werase(nav);

    draw_border(nav, "Navigator", active ? COL_SELECT : COL_BORDER);

    /* scroll offset — keep cursor visible */
    int max_rows = rows - 2;
    int scroll = 0;
    if (cursor >= max_rows)
        scroll = cursor - max_rows + 1;

    for (int i = 0; i < max_rows && (i + scroll) < vis_count; i++)
    {
        int ei = visible[i + scroll];
        Entry* e = &ENTRIES[ei];
        int row = i + 1;
        int col = 2;

        if (i + scroll == cursor)
        {
            wattron(nav, COLOR_PAIR(COL_SELECT) | A_BOLD);
        }
        else if (e->is_folder)
        {
            wattron(nav, COLOR_PAIR(COL_FOLDER) | A_BOLD);
        }
        else
        {
            wattron(nav, COLOR_PAIR(COL_ITEM));
        }

        wmove(nav, row, col);
        wclrtoeol(nav);

        if (e->is_folder)
        {
            mvwprintw(nav, row, col, "%s %s", e->expanded ? "v" : ">", e->name);
        }
        else
        {
            mvwprintw(nav, row, col + 2, "  %s", e->name);
        }

        if (i + scroll == cursor)
        {
            wattroff(nav, COLOR_PAIR(COL_SELECT) | A_BOLD);
        }
        else if (e->is_folder)
        {
            wattroff(nav, COLOR_PAIR(COL_FOLDER) | A_BOLD);
        }
        else
        {
            wattroff(nav, COLOR_PAIR(COL_ITEM));
        }

        (void)cols;
    }

    wrefresh(nav);
}

static void draw_viz(WINDOW* viz, State* s, int* visible, int cursor, int active)
{
    int rows, cols;
    getmaxyx(viz, rows, cols);
    werase(viz);

    draw_border(viz, "Visualizer", active ? COL_SELECT : COL_BORDER);

    int mid_row = rows / 2;
    int mid_col = cols / 2;

    if (!s->demo_ran)
    {
        /* idle state */
        wattron(viz, COLOR_PAIR(COL_VIZPANE) | A_BOLD);
        mvwprintw(viz, mid_row - 2, mid_col - 12, "  C DSA Interactive Suite  ");
        wattroff(viz, COLOR_PAIR(COL_VIZPANE) | A_BOLD);

        wattron(viz, COLOR_PAIR(COL_ITEM));
        mvwprintw(viz, mid_row, mid_col - 14, "Select an algorithm and press Enter");
        mvwprintw(viz, mid_row + 1, mid_col - 12, "Tab to switch panes");
        mvwprintw(viz, mid_row + 2, mid_col - 12, "<- -> or Space to expand/collapse");
        mvwprintw(viz, mid_row + 3, mid_col - 12, "q to quit");
        wattroff(viz, COLOR_PAIR(COL_ITEM));
    }
    else
    {
        /* show last ran info */
        wattron(viz, COLOR_PAIR(COL_VIZPANE) | A_BOLD);
        mvwprintw(viz, 2, 3, "Last run: %s", s->last_ran);
        wattroff(viz, COLOR_PAIR(COL_VIZPANE) | A_BOLD);

        wattron(viz, COLOR_PAIR(COL_ITEM));
        mvwprintw(viz, 4, 3, "Demo ran in full terminal mode.");
        mvwprintw(viz, 5, 3, "Press Enter again to re-run.");
        mvwprintw(viz, 7, 3, "Note: interactive demos require");
        mvwprintw(viz, 8, 3, "full terminal I/O. The visualizer");
        mvwprintw(viz, 9, 3, "pane will show step-by-step output");
        mvwprintw(viz, 10, 3, "once demos are refactored to return");
        mvwprintw(viz, 11, 3, "structured data.");
        wattroff(viz, COLOR_PAIR(COL_ITEM));
    }

    /* preview selected item name at bottom */
    int ei = visible[cursor];
    if (!ENTRIES[ei].is_folder && ENTRIES[ei].fn != NULL)
    {
        wattron(viz, COLOR_PAIR(COL_STATUS));
        mvwprintw(viz, rows - 2, 2, " Ready: %-30s ", ENTRIES[ei].name);
        wattroff(viz, COLOR_PAIR(COL_STATUS));
    }

    wrefresh(viz);

    (void)mid_col;
    (void)mid_row;
}

static void draw_status(WINDOW* status, ActivePane active)
{
    int cols;
    getmaxyx(status, (int){0}, cols);
    werase(status);
    wattron(status, COLOR_PAIR(COL_STATUS));

    if (active == PANE_NAV)
    {
        mvwprintw(status, 0, 0,
                  " Up/Down Navigate  Enter Run  Space/<- -> Expand  Tab Switch pane  q Quit");
    }
    else
    {
        mvwprintw(status, 0, 0, " Tab Switch pane  q Quit");
    }

    /* pad to full width */
    int len = getcurx(status);
    for (int i = len; i < cols; i++)
        waddch(status, ' ');

    wattroff(status, COLOR_PAIR(COL_STATUS));
    wrefresh(status);

    (void)cols;
}

/* ── run a demo ─────────────────────────────────────────────────────────────── */
static void run_demo(demo_fn fn, const char* name, State* s)
{
    /* hand terminal back to normal mode */
    endwin();

    printf("\n\033[1;36m═══ %s ═══\033[0m\n\n", name);
    fn();
    printf("\n\033[1;36m═══ End of %s ═══\033[0m\n", name);
    printf("\nPress Enter to return to menu...");
    fflush(stdout);

    /* wait for Enter */
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    /* reclaim terminal */
    refresh();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    /* update state */
    strncpy(s->last_ran, name, sizeof(s->last_ran) - 1);
    s->last_ran[sizeof(s->last_ran) - 1] = '\0';
    s->demo_ran = 1;
}

/* ── main loop ──────────────────────────────────────────────────────────────── */
void tui_run(void)
{
    // setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    init_colors();

    int visible[128];
    int vis_count;

    State s = {.nav_cursor = 0, .active = PANE_NAV, .last_ran = {0}, .demo_ran = 0};

    int ch;

    while (1)
    {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        /* layout: 1/3 nav | 2/3 viz | 1 row status */
        int nav_w = cols / 3;
        int viz_w = cols - nav_w;
        int content_h = rows - 1; /* leave 1 row for status bar */

        WINDOW* nav_win = newwin(content_h, nav_w, 0, 0);
        WINDOW* viz_win = newwin(content_h, viz_w, 0, nav_w);
        WINDOW* status_win = newwin(1, cols, rows - 1, 0);

        keypad(nav_win, TRUE);
        keypad(viz_win, TRUE);

        /* build visible list */
        vis_count = build_visible(visible, 128);

        /* clamp cursor */
        if (s.nav_cursor >= vis_count)
            s.nav_cursor = vis_count - 1;
        if (s.nav_cursor < 0)
            s.nav_cursor = 0;

        draw_nav(nav_win, visible, vis_count, s.nav_cursor, s.active == PANE_NAV);
        draw_viz(viz_win, &s, visible, s.nav_cursor, s.active == PANE_VIZ);
        draw_status(status_win, s.active);

        /* input from active pane */
        WINDOW* active_win = (s.active == PANE_NAV) ? nav_win : viz_win;
        ch = wgetch(active_win);

        switch (ch)
        {

            case 'q':
            case 'Q':
                delwin(nav_win);
                delwin(viz_win);
                delwin(status_win);
                endwin();
                return;

            case '\t':
                /* switch active pane */
                s.active = (s.active == PANE_NAV) ? PANE_VIZ : PANE_NAV;
                break;

            case KEY_UP:
            case 'k':
                if (s.active == PANE_NAV && s.nav_cursor > 0)
                    s.nav_cursor--;
                break;

            case KEY_DOWN:
            case 'j':
                if (s.active == PANE_NAV && s.nav_cursor < vis_count - 1)
                    s.nav_cursor++;
                break;

            case KEY_LEFT:
            case 'h':
            {
                /* collapse folder or jump to parent */
                int ei = visible[s.nav_cursor];
                if (ENTRIES[ei].is_folder && ENTRIES[ei].expanded)
                {
                    ENTRIES[ei].expanded = 0;
                }
                else
                {
                    /* jump to parent folder */
                    int parent = find_parent(ei);
                    if (parent >= 0)
                    {
                        /* find parent in visible list */
                        vis_count = build_visible(visible, 128);
                        for (int i = 0; i < vis_count; i++)
                        {
                            if (visible[i] == parent)
                            {
                                s.nav_cursor = i;
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case KEY_RIGHT:
            case 'l':
            case ' ':
            {
                /* expand folder */
                int ei = visible[s.nav_cursor];
                if (ENTRIES[ei].is_folder)
                    ENTRIES[ei].expanded = !ENTRIES[ei].expanded;
                break;
            }

            case '\n':
            case KEY_ENTER:
            {
                int ei = visible[s.nav_cursor];
                Entry* e = &ENTRIES[ei];

                if (e->is_folder)
                {
                    /* toggle expand */
                    e->expanded = !e->expanded;
                }
                else if (e->fn != NULL)
                {
                    /* run demo */
                    delwin(nav_win);
                    delwin(viz_win);
                    delwin(status_win);
                    run_demo(e->fn, e->name, &s);
                    /* windows recreated next loop iteration */
                    continue;
                }
                break;
            }

            default:
                break;
        }

        delwin(nav_win);
        delwin(viz_win);
        delwin(status_win);
    }
}
