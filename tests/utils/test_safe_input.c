#include "safe_input.h"
#include "mock_printf.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printf mock_printf

static void write_mock_input(const char* content)
{
    FILE* f = fopen("temp_input.txt", "w");
    assert(f != NULL);
    if (content)
    {
        fputs(content, f);
    }
    fclose(f);
    FILE* redirected = freopen("temp_input.txt", "r", stdin);
    assert(redirected != NULL);
}

static void cleanup_mock_input(void)
{
    // Reopen standard input from console if possible, or just remove temp file.
    // On tests, we don't need to restore stdin to actual terminal, but let's clean up files.
    remove("temp_input.txt");
}

void test_safe_input_int(void)
{
    int val = 0;
    int status;

    // Test case 1: Valid input in range
    reset_printf_buf();
    write_mock_input("5\n");
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == 1);
    assert(val == 5);

    // Test case 2: Out of range input (too low)
    reset_printf_buf();
    write_mock_input("0\n");
    val = 999;
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == 0);
    assert(val == 999); // val unchanged on failure

    // Test case 3: Out of range input (too high)
    reset_printf_buf();
    write_mock_input("11\n");
    val = 999;
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == 0);
    assert(val == 999);

    // Test case 4: Non-integer input
    reset_printf_buf();
    write_mock_input("abc\n");
    val = 999;
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == 0);
    assert(val == 999);

    // Test case 5: Exit signal (-1)
    reset_printf_buf();
    write_mock_input("-1\n");
    val = 999;
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == INPUT_EXIT_SIGNAL);

    // Test case 6: EOF on empty stream
    reset_printf_buf();
    write_mock_input("");
    val = 999;
    status = safe_input_int(&val, "Enter number: ", 1, 10);
    assert(status == INPUT_EXIT_SIGNAL);
}

void test_safe_input_string(void)
{
    char buf[10];
    int status;

    // Test case 1: Valid string
    reset_printf_buf();
    write_mock_input("hello\n");
    status = safe_input_string(buf, sizeof(buf), "Enter string: ");
    assert(status == 1);
    assert(strcmp(buf, "hello") == 0);

    // Test case 2: Exit signal (X)
    reset_printf_buf();
    write_mock_input("X\n");
    status = safe_input_string(buf, sizeof(buf), "Enter string: ");
    assert(status == INPUT_EXIT_SIGNAL);

    // Test case 3: String with spaces (should retry and then succeed/EOF)
    reset_printf_buf();
    write_mock_input("hello world\nworld\n");
    status = safe_input_string(buf, sizeof(buf), "Enter string: ");
    assert(status == 1);
    assert(strcmp(buf, "world") == 0);

    // Test case 4: Empty string retries
    reset_printf_buf();
    write_mock_input("\n\ntest\n");
    status = safe_input_string(buf, sizeof(buf), "Enter string: ");
    assert(status == 1);
    assert(strcmp(buf, "test") == 0);

    // Test case 5: EOF handling
    reset_printf_buf();
    write_mock_input("");
    status = safe_input_string(buf, sizeof(buf), "Enter string: ");
    assert(status == INPUT_EXIT_SIGNAL);

    // Test case 6: Exceeding buffer size (should truncate and flush remainder)
    reset_printf_buf();
    // Buffer size is 5, so max read length is 4 characters.
    // "longerstring" -> read "long", flush "erstring\n", next read should get "next"
    write_mock_input("longerstring\nnext\n");
    char short_buf[5];
    status = safe_input_string(short_buf, sizeof(short_buf), "Enter short: ");
    assert(status == 1);
    assert(strcmp(short_buf, "long") == 0);

    // Verify stdin was flushed properly by reading the next string
    status = safe_input_string(short_buf, sizeof(short_buf), "Enter next: ");
    assert(status == 1);
    assert(strcmp(short_buf, "next") == 0);
}

int main(void)
{
    test_safe_input_int();
    test_safe_input_string();
    cleanup_mock_input();

    #undef printf
    fprintf(stdout, "All safe input tests passed successfully!\n");
    return 0;
}
