#include "telemetry.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
static void ensure_dir_exists(const char* path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        mkdir(path, 0755);
    }
}

static void test_telemetry_enable_disable(void)
{
    set_telemetry_enabled(false);
    assert(!is_telemetry_enabled());

    set_telemetry_enabled(true);
    assert(is_telemetry_enabled());
}

static void test_telemetry_filepath(void)
{
    set_telemetry_filepath("test_binaries/test_trace.json");
    assert(strcmp(get_telemetry_filepath(), "test_binaries/test_trace.json") == 0);
}

static void test_telemetry_logging_flow(void)
{
    // Clean old test file if exists
    ensure_dir_exists("test_binaries");
    remove("test_binaries/test_trace.json");

    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/test_trace.json");

    telemetry_init("bubble_sort");

    int arr1[] = {5, 3, 8};
    telemetry_log_step(arr1, 3, "Initial array");

    int arr2[] = {3, 5, 8};
    telemetry_log_step(arr2, 3, "Swapped 5 and 3");

    telemetry_close();

    // Verify file contents
    FILE* fp = fopen("test_binaries/test_trace.json", "r");
    assert(fp != NULL);

    char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);

    // Verify it contains trace components
    assert(strstr(buffer, "\"algorithm\": \"bubble_sort\"") != NULL);
    assert(strstr(buffer, "\"step\": 0") != NULL);
    assert(strstr(buffer, "\"step\": 1") != NULL);
    assert(strstr(buffer, "\"array\": [5, 3, 8]") != NULL);
    assert(strstr(buffer, "\"array\": [3, 5, 8]") != NULL);
    assert(strstr(buffer, "\"message\": \"Initial array\"") != NULL);
    assert(strstr(buffer, "\"message\": \"Swapped 5 and 3\"") != NULL);

    // Check JSON brackets/formatting
    assert(buffer[0] == '[');
    // Find last character (skipping whitespace/newlines)
    int len = strlen(buffer);
    while (len > 0 &&
           (buffer[len - 1] == '\n' || buffer[len - 1] == ' ' || buffer[len - 1] == '\r'))
    {
        len--;
    }
    assert(buffer[len - 1] == ']');

    // Clean up
    remove("test_binaries/test_trace.json");
}

static void test_telemetry_disabled_flow(void)
{
    ensure_dir_exists("test_binaries");
    remove("test_binaries/test_disabled.json");

    set_telemetry_enabled(false);
    set_telemetry_filepath("test_binaries/test_disabled.json");

    telemetry_init("test");
    int arr[] = {1, 2};
    telemetry_log_step(arr, 2, "Should not log");
    telemetry_close();

    FILE* fp = fopen("test_binaries/test_disabled.json", "r");
    assert(fp == NULL); // File should not be created
}

int main(void)
{
    printf("Starting Telemetry unit tests...\n");
    test_telemetry_enable_disable();
    test_telemetry_filepath();
    test_telemetry_logging_flow();
    test_telemetry_disabled_flow();
    printf("All Telemetry unit tests passed successfully!\n");
    return 0;
}
