#include "concurrent_data_structures.h"
#include "ring_buffer.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

#define BUFFER_CAPACITY 5
#define TOTAL_ITEMS 10

typedef struct {
    ring_buffer_t *rb;
    int id;
} thread_data_t;

#if defined(_WIN32)
static DWORD WINAPI producer_thread(LPVOID arg)
#else
static void* producer_thread(void *arg)
#endif
{
    thread_data_t *tdata = (thread_data_t*)arg;
    for (int i = 1; i <= TOTAL_ITEMS; i++) {
        int *item = (int*)malloc(sizeof(int));
        if (item) {
            *item = i + (tdata->id * 100);
            printf("\n\033[1;33m[Producer %d]\033[0m Produced Item: %d. Waiting to enqueue...", tdata->id, *item);
            
            rb_enqueue_blocking(tdata->rb, item);
            printf("\n\033[1;32m[Producer %d]\033[0m Enqueued Item: %d (Count: %zu)", tdata->id, *item, rb_get_count(tdata->rb));
        }
        sleep_seconds(0.4f);
    }
#if defined(_WIN32)
    return 0;
#else
    return NULL;
#endif
}

#if defined(_WIN32)
static DWORD WINAPI consumer_thread(LPVOID arg)
#else
static void* consumer_thread(void *arg)
#endif
{
    thread_data_t *tdata = (thread_data_t*)arg;
    for (int i = 1; i <= TOTAL_ITEMS; i++) {
        int *item = NULL;
        printf("\n\033[1;34m[Consumer %d]\033[0m Waiting to dequeue...", tdata->id);
        
        rb_dequeue_blocking(tdata->rb, (void**)&item);
        if (item) {
            printf("\n\033[1;35m[Consumer %d]\033[0m Dequeued Item: %d (Count: %zu)", tdata->id, *item, rb_get_count(tdata->rb));
            free(item);
        }
        sleep_seconds(0.7f);
    }
#if defined(_WIN32)
    return 0;
#else
    return NULL;
#endif
}

static void run_ring_buffer_simulation(void) {
    printf("\n\033[1;36m=== Thread-Safe Ring Buffer Simulation ===\033[0m\n");
    printf("Starting 1 Producer and 1 Consumer...\n");

    ring_buffer_t *rb = rb_create(BUFFER_CAPACITY);
    if (!rb) {
        printf("Failed to create ring buffer!\n");
        return;
    }

    thread_data_t prod_data = {rb, 1};
    thread_data_t cons_data = {rb, 1};

#if defined(_WIN32)
    HANDLE hProducer = CreateThread(NULL, 0, producer_thread, &prod_data, 0, NULL);
    HANDLE hConsumer = CreateThread(NULL, 0, consumer_thread, &cons_data, 0, NULL);
    if (hProducer) WaitForSingleObject(hProducer, INFINITE);
    if (hConsumer) WaitForSingleObject(hConsumer, INFINITE);
    if (hProducer) CloseHandle(hProducer);
    if (hConsumer) CloseHandle(hConsumer);
#else
    pthread_t prod_tid, cons_tid;
    pthread_create(&prod_tid, NULL, producer_thread, &prod_data);
    pthread_create(&cons_tid, NULL, consumer_thread, &cons_data);
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);
#endif

    printf("\n\nSimulation complete.\n");
    rb_destroy(rb);
}

void concurrent_data_structures_demo(void) {
    while (1) {
        if (!is_instant()) {
            clear_screen();
        }
        printf("\n\033[1;36m=== CONCURRENT DATA STRUCTURES DEMO ===\033[0m\n");
        printf("1. Thread-Safe Generic Ring Buffer (Producer-Consumer Demo)\n");
        printf("Enter option (-1 to return to main menu): ");
        fflush(stdout);

        int choice;
        int status = safe_input_int(&choice, "", 1, 1);
        if (status == INPUT_EXIT_SIGNAL || choice == -1) {
            break;
        }
        if (status == 0) continue;

        if (choice == 1) {
            run_ring_buffer_simulation();
            printf("\nPress Enter to continue...");
            getchar(); // Consume remaining newline if necessary
        }
    }
}
