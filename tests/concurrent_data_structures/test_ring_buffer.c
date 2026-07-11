#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ring_buffer.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

// Basic single-threaded tests
static void test_create_destroy() {
    printf("Running test_create_destroy...\n");
    ring_buffer_t *rb = rb_create(5);
    assert(rb != NULL);
    assert(rb_get_capacity(rb) == 5);
    assert(rb_get_count(rb) == 0);
    assert(rb_is_empty(rb));
    assert(!rb_is_full(rb));
    rb_destroy(rb);
    printf("Passed test_create_destroy.\n");
}

static void test_enqueue_dequeue() {
    printf("Running test_enqueue_dequeue...\n");
    ring_buffer_t *rb = rb_create(3);
    assert(rb != NULL);

    int a = 1, b = 2, c = 3, d = 4;

    assert(rb_enqueue(rb, &a) == RB_SUCCESS);
    assert(rb_enqueue(rb, &b) == RB_SUCCESS);
    assert(rb_enqueue(rb, &c) == RB_SUCCESS);
    
    assert(rb_is_full(rb));
    assert(rb_enqueue(rb, &d) == RB_ERROR_FULL);

    void *out = NULL;
    assert(rb_dequeue(rb, &out) == RB_SUCCESS);
    assert(*(int*)out == 1);
    assert(!rb_is_full(rb));
    
    assert(rb_enqueue(rb, &d) == RB_SUCCESS);
    assert(rb_is_full(rb));
    
    assert(rb_dequeue(rb, &out) == RB_SUCCESS);
    assert(*(int*)out == 2);
    assert(rb_dequeue(rb, &out) == RB_SUCCESS);
    assert(*(int*)out == 3);
    assert(rb_dequeue(rb, &out) == RB_SUCCESS);
    assert(*(int*)out == 4);

    assert(rb_is_empty(rb));
    assert(rb_dequeue(rb, &out) == RB_ERROR_EMPTY);

    rb_destroy(rb);
    printf("Passed test_enqueue_dequeue.\n");
}

static void test_edge_cases() {
    printf("Running test_edge_cases...\n");
    // Test 0 capacity
    ring_buffer_t *rb0 = rb_create(0);
    assert(rb0 == NULL);

    // Test NULL buffer
    int val = 42;
    void *out;
    assert(rb_enqueue(NULL, &val) == RB_ERROR_NULL_POINTER);
    assert(rb_dequeue(NULL, &out) == RB_ERROR_NULL_POINTER);
    assert(rb_enqueue_blocking(NULL, &val) == RB_ERROR_NULL_POINTER);
    assert(rb_dequeue_blocking(NULL, &out) == RB_ERROR_NULL_POINTER);

    assert(rb_get_capacity(NULL) == 0);
    assert(rb_get_count(NULL) == 0);
    assert(rb_is_empty(NULL) == true);
    assert(rb_is_full(NULL) == false);

    ring_buffer_t *rb = rb_create(2);
    assert(rb != NULL);

    // Test NULL out pointer
    assert(rb_dequeue(rb, NULL) == RB_ERROR_NULL_POINTER);

    // Test wrap around
    int data1 = 1, data2 = 2, data3 = 3, data4 = 4;
    assert(rb_enqueue(rb, &data1) == RB_SUCCESS); // 1
    assert(rb_enqueue(rb, &data2) == RB_SUCCESS); // 1, 2
    assert(rb_is_full(rb) == true);

    assert(rb_dequeue(rb, &out) == RB_SUCCESS); // 2
    assert(*(int*)out == 1);
    assert(rb_is_full(rb) == false);

    assert(rb_enqueue(rb, &data3) == RB_SUCCESS); // 2, 3 (wrapped)
    assert(rb_is_full(rb) == true);

    assert(rb_dequeue(rb, &out) == RB_SUCCESS); // 3
    assert(*(int*)out == 2);

    assert(rb_dequeue(rb, &out) == RB_SUCCESS); // empty
    assert(*(int*)out == 3);

    assert(rb_is_empty(rb) == true);
    assert(rb_dequeue(rb, &out) == RB_ERROR_EMPTY);

    rb_destroy(rb);
    printf("Passed test_edge_cases.\n");
}

// Simple multi-threaded test structure
#define THREAD_ITEMS 100
#define BUFFER_CAPACITY 10

typedef struct {
    ring_buffer_t *rb;
} mt_test_t;

#if defined(_WIN32)
static DWORD WINAPI producer_thread(LPVOID arg)
#else
static void* producer_thread(void *arg)
#endif
{
    mt_test_t *tdata = (mt_test_t*)arg;
    for (int i = 0; i < THREAD_ITEMS; i++) {
        int *val = (int*)malloc(sizeof(int));
        *val = i;
        rb_enqueue_blocking(tdata->rb, val);
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
    mt_test_t *tdata = (mt_test_t*)arg;
    for (int i = 0; i < THREAD_ITEMS; i++) {
        int *val = NULL;
        rb_dequeue_blocking(tdata->rb, (void**)&val);
        assert(val != NULL);
        assert(*val == i);
        free(val);
    }
#if defined(_WIN32)
    return 0;
#else
    return NULL;
#endif
}

static void test_multithreaded() {
    printf("Running test_multithreaded...\n");
    ring_buffer_t *rb = rb_create(BUFFER_CAPACITY);
    assert(rb != NULL);

    mt_test_t tdata = {rb};

#if defined(_WIN32)
    HANDLE hProd = CreateThread(NULL, 0, producer_thread, &tdata, 0, NULL);
    HANDLE hCons = CreateThread(NULL, 0, consumer_thread, &tdata, 0, NULL);
    WaitForSingleObject(hProd, INFINITE);
    WaitForSingleObject(hCons, INFINITE);
    CloseHandle(hProd);
    CloseHandle(hCons);
#else
    pthread_t prod_tid, cons_tid;
    pthread_create(&prod_tid, NULL, producer_thread, &tdata);
    pthread_create(&cons_tid, NULL, consumer_thread, &tdata);
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);
#endif

    assert(rb_is_empty(rb));
    rb_destroy(rb);
    printf("Passed test_multithreaded.\n");
}

int main() {
    printf("--- Running Ring Buffer Tests ---\n");
    test_create_destroy();
    test_enqueue_dequeue();
    test_edge_cases();
    test_multithreaded();
    printf("--- All Tests Passed ---\n");
    return 0;
}
