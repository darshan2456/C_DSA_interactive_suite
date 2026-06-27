#include "../utils/config.h"
#include "clear_screen.h"
#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR 256

typedef struct ACNode {
    struct ACNode* children[MAX_CHAR];
    struct ACNode* fail;
    int* output;
    int output_count;
} ACNode;

static ACNode* create_ac_node(void)
{
    ACNode* node = (ACNode*)malloc(sizeof(ACNode));
    if (node)
    {
        for (int i = 0; i < MAX_CHAR; i++)
            node->children[i] = NULL;
        node->fail = NULL;
        node->output = NULL;
        node->output_count = 0;
    }
    return node;
}

static void insert_ac_pattern(ACNode* root, const char* pattern, int pat_idx)
{
    ACNode* curr = root;
    for (int i = 0; pattern[i] != '\0'; i++)
    {
        unsigned char c = pattern[i];
        if (curr->children[c] == NULL)
            curr->children[c] = create_ac_node();
        curr = curr->children[c];
    }
    curr->output = (int*)realloc(curr->output, (curr->output_count + 1) * sizeof(int));
    curr->output[curr->output_count++] = pat_idx;
}

typedef struct QueueNode {
    ACNode* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

static void enqueue(Queue* q, ACNode* node)
{
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->data = node;
    temp->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

static ACNode* dequeue(Queue* q)
{
    if (q->front == NULL)
        return NULL;
    QueueNode* temp = q->front;
    ACNode* node = temp->data;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return node;
}

static void build_fail_links(ACNode* root)
{
    Queue q = {NULL, NULL};

    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (root->children[i] != NULL)
        {
            root->children[i]->fail = root;
            enqueue(&q, root->children[i]);
        }
    }

    while (q.front != NULL)
    {
        ACNode* curr = dequeue(&q);

        for (int i = 0; i < MAX_CHAR; i++)
        {
            if (curr->children[i] != NULL)
            {
                ACNode* child = curr->children[i];
                ACNode* f = curr->fail;

                while (f != NULL && f->children[i] == NULL)
                {
                    f = f->fail;
                }

                if (f == NULL)
                    child->fail = root;
                else
                    child->fail = f->children[i];

                if (child->fail->output_count > 0)
                {
                    int old_cnt = child->output_count;
                    int add_cnt = child->fail->output_count;
                    child->output = (int*)realloc(child->output, (old_cnt + add_cnt) * sizeof(int));
                    for (int j = 0; j < add_cnt; j++)
                    {
                        child->output[old_cnt + j] = child->fail->output[j];
                    }
                    child->output_count += add_cnt;
                }

                enqueue(&q, child);
            }
        }
    }
}

static void free_ac_trie(ACNode* node)
{
    if (node == NULL)
        return;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (node->children[i] != NULL)
            free_ac_trie(node->children[i]);
    }
    free(node->output);
    free(node);
}

void aho_corasick_search(char* text, char** patterns, int k)
{
    ACNode* root = create_ac_node();
    for (int i = 0; i < k; i++)
    {
        insert_ac_pattern(root, patterns[i], i);
    }
    build_fail_links(root);

    ACNode* curr = root;
    int found = 0;
    int n = strlen(text);

    for (int i = 0; i < n; i++)
    {
        unsigned char c = text[i];
        while (curr != NULL && curr->children[c] == NULL)
        {
            curr = curr->fail;
        }
        if (curr == NULL)
            curr = root;
        else
            curr = curr->children[c];

        for (int j = 0; j < curr->output_count; j++)
        {
            int pat_idx = curr->output[j];
            int pat_len = strlen(patterns[pat_idx]);
            printf("Pattern %s found at index %d\n", patterns[pat_idx], i - pat_len + 1);
            found++;
        }
    }

    free_ac_trie(root);

    if (found == 0)
    {
        printf("No patterns found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void aho_corasick_visualization(char* text, char** patterns, int k)
{
    ACNode* root = create_ac_node();
    for (int i = 0; i < k; i++)
    {
        insert_ac_pattern(root, patterns[i], i);
    }
    build_fail_links(root);

    ACNode* curr = root;
    int found = 0;
    int n = strlen(text);
    int step = 1;

    for (int i = 0; i < n; i++)
    {
        if (!is_instant())
        {
            clear_screen();
        }

        printf("\nStep %d\n", step++);
        printf("Text: %s\n", text);
        printf("Scan: ");
        for (int p = 0; p < i; p++)
            printf(" ");
        printf("^ (Current character: '%c')\n", text[i]);

        unsigned char c = text[i];
        ACNode* prev = curr;
        while (curr != NULL && curr->children[c] == NULL)
        {
            curr = curr->fail;
        }
        if (curr == NULL)
        {
            curr = root;
            printf("State transition: character '%c' mismatch, following fail link to root\n", c);
        }
        else
        {
            curr = curr->children[c];
            printf("State transition: matched '%c', moving from state %p to state %p\n", c, (void*)prev, (void*)curr);
        }

        if (curr->output_count > 0)
        {
            printf("\nMatches found at this state:\n");
            for (int j = 0; j < curr->output_count; j++)
            {
                int pat_idx = curr->output[j];
                int pat_len = strlen(patterns[pat_idx]);
                printf("- Pattern '%s' found at index %d\n", patterns[pat_idx], i - pat_len + 1);
                found++;
            }
        }
        dynamic_sleep();
    }

    free_ac_trie(root);

    printf("\n-----------------------------------\n");
    if (found == 0)
    {
        printf("No patterns found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void aho_corasick_demo(void)
{
    while (1)
    {
        char text[100];
        int num_patterns = 0;

        printf("\nAho-Corasick Multi-Pattern Matching Demo\n");

        int status_T =
            safe_input_string(text, "Enter text (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_T == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting demo...\n");
            return;
        }

        int status_K =
            safe_input_int(&num_patterns, "Enter number of patterns to search (1 to 5), or -1 to exit: ", 1, 5);
        if (status_K == INPUT_EXIT_SIGNAL || num_patterns == -1)
        {
            printf("\nExiting demo...\n");
            return;
        }

        char** patterns = (char**)malloc(num_patterns * sizeof(char*));
        if (!patterns)
            return;

        int exit_requested = 0;
        for (int i = 0; i < num_patterns; i++)
        {
            patterns[i] = (char*)malloc(100 * sizeof(char));
            if (!patterns[i])
            {
                exit_requested = 1;
                break;
            }
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter pattern %d: ", i + 1);
            int status_P = safe_input_string(patterns[i], prompt);
            if (status_P == INPUT_EXIT_SIGNAL)
            {
                exit_requested = 1;
                break;
            }
        }

        if (!exit_requested)
        {
            clock_t start_t = clock();
            aho_corasick_visualization(text, patterns, num_patterns);
            clock_t end_t = clock();
            double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

            printf("Total CPU time taken: %f seconds\n", total_t);
            add_to_history("Aho-Corasick Search", strlen(text), total_t);
        }

        for (int i = 0; i < num_patterns; i++)
        {
            if (patterns[i])
                free(patterns[i]);
        }
        free(patterns);

        if (exit_requested)
        {
            printf("\nExiting demo...\n");
            return;
        }
    }
}
