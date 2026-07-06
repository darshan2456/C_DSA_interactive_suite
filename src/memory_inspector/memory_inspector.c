#include "memory_inspector.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void print_hexdump(const void* ptr, size_t size)
{
    print_hexdump_ex(ptr, size, 16);
}

void print_hexdump_ex(const void* ptr, size_t size, int bytes_per_line)
{
    if (ptr == NULL || size == 0)
    {
        printf("  [NULL pointer or zero size]\n");
        return;
    }

    const unsigned char* bytes = (const unsigned char*)ptr;

    printf("\n");
    printf("  +--------------------------------------------------+\n");
    printf("  |  Memory Hexdump: %zu bytes                       |\n", size);
    printf("  +--------------------------------------------------+\n");
    printf("  |\n");

    size_t line_count = 0;
    for (size_t i = 0; i < size; i += bytes_per_line)
    {
        // Print address
        printf("  |  %p  ", (void*)(bytes + i));

        // Print hex bytes
        for (int j = 0; j < bytes_per_line; j++)
        {
            if (i + j < size)
            {
                printf("%02x ", bytes[i + j]);
            }
            else
            {
                printf("   ");
            }

            // Extra space after 8 bytes for readability
            if (j == 7)
            {
                printf(" ");
            }
        }

        printf(" | ");

        // Print ASCII representation
        for (int j = 0; j < bytes_per_line && (i + j) < size; j++)
        {
            unsigned char c = bytes[i + j];
            if (c >= 32 && c <= 126)
            {
                printf("%c", c);
            }
            else
            {
                printf(".");
            }
        }

        printf(" |\n");
        line_count++;
    }

    printf("  |\n");
    printf("  +--------------------------------------------------+\n");
    printf("\n");
    printf("  Legend: ASCII shown on right, '.' for non-printable\n");
    printf("          Address shown on left\n");
    printf("\n");
}

void print_memory_as_types(const void* ptr, size_t size)
{
    if (ptr == NULL)
    {
        printf("  [NULL pointer]\n");
        return;
    }

    const unsigned char* bytes = (const unsigned char*)ptr;

    printf("\n");
    printf("  +--------------------------------------------------+\n");
    printf("  |  Memory Interpretation (Little Endian)            |\n");
    printf("  +--------------------------------------------------+\n");
    printf("\n");

    // Interpret as different integer types
    if (size >= 1)
    {
        uint8_t val8 = *((const uint8_t*)ptr);
        printf("  uint8_t  (1 byte):  %u (0x%02x)\n", val8, val8);
    }

    if (size >= 2)
    {
        uint16_t val16 = *((const uint16_t*)ptr);
        printf("  uint16_t (2 bytes): %u (0x%04x)\n", val16, val16);
    }

    if (size >= 4)
    {
        uint32_t val32 = *((const uint32_t*)ptr);
        printf("  uint32_t (4 bytes): %u (0x%08x)\n", val32, val32);
    }

    if (size >= 8)
    {
        uint64_t val64 = *((const uint64_t*)ptr);
        printf("  uint64_t (8 bytes): %lu (0x%016lx)\n", (unsigned long)val64, (unsigned long)val64);
    }

    // Float interpretation
    if (size >= 4)
    {
        float fval = *((const float*)ptr);
        printf("\n  float   (4 bytes): %f\n", fval);
    }

    if (size >= 8)
    {
        double dval = *((const double*)ptr);
        printf("  double  (8 bytes): %lf\n", dval);
    }

    // Pointer interpretation (assuming 64-bit)
    if (size >= 8)
    {
        void* pval = *((void**)ptr);
        printf("\n  void*   (8 bytes): %p\n", pval);
    }

    printf("\n");
    printf("  Byte sequence (hex): ");
    for (size_t i = 0; i < size && i < 16; i++)
    {
        printf("%02x ", bytes[i]);
    }
    printf("\n");
    printf("\n");
    printf("  Note: Your system uses %s-endian byte order.\n",
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
           "little"
#else
           "big"
#endif
#else
           "unknown"
#endif
    );
    printf("\n");
}

int compare_memory(const void* ptr1, const void* ptr2, size_t size)
{
    if (ptr1 == NULL || ptr2 == NULL)
    {
        return -1;
    }

    const unsigned char* b1 = (const unsigned char*)ptr1;
    const unsigned char* b2 = (const unsigned char*)ptr2;

    int differences = 0;

    printf("\n");
    printf("  +--------------------------------------------------+\n");
    printf("  |  Memory Comparison: %zu bytes                      |\n", size);
    printf("  +--------------------------------------------------+\n");
    printf("\n");

    for (size_t i = 0; i < size; i++)
    {
        if (b1[i] != b2[i])
        {
            differences++;
            if (differences <= 10) // Show first 10 differences
            {
                printf("  Difference at offset %zu:\n", i);
                printf("    Address %p: %02x != %02x\n",
                       (void*)(b1 + i), b1[i], b2[i]);
            }
        }
    }

    if (differences == 0)
    {
        printf("  [Memory regions are IDENTICAL]\n");
    }
    else if (differences > 10)
    {
        printf("  ... and %d more differences\n", differences - 10);
    }

    printf("\n");
    printf("  Total differences: %d / %zu bytes\n", differences, size);
    printf("\n");

    return differences;
}

static void demo_basic_hexdump(void)
{
    printf("\n");
    printf("  Example: Integer array in memory\n");
    printf("  --------------------------------\n");

    int numbers[] = {0x01234567, 0x89ABCDEF, 0xDEADBEEF, 0xCAFEBABE};
    size_t arr_size = sizeof(numbers);

    printf("\n  Integer array: {0x01234567, 0x89ABCDEF, 0xDEADBEEF, 0xCAFEBABE}\n");
    printf("  Total size: %zu bytes\n", arr_size);
    printf("  Number of elements: %zu\n\n", sizeof(numbers) / sizeof(numbers[0]));

    print_hexdump(numbers, arr_size);
}

static void demo_pointer_sizes(void)
{
    printf("\n");
    printf("  Pointer Size Demonstration\n");
    printf("  ---------------------------\n");

    printf("\n  sizeof(char*):     %zu bytes\n", sizeof(char*));
    printf("  sizeof(int*):      %zu bytes\n", sizeof(int*));
    printf("  sizeof(void*):    %zu bytes\n", sizeof(void*));
    printf("  sizeof(struct*):  %zu bytes (assumes 8-byte alignment)\n", sizeof(void*));

    int value = 42;
    int* ptr = &value;
    double dval = 3.14159;
    char str[] = "Hello";

    printf("\n");
    printf("  Memory of variables:\n");
    printf("  --------------------\n");
    printf("  int value = 42;\n");
    print_hexdump(&value, sizeof(int));

    printf("  int* ptr = &value;\n");
    print_hexdump(&ptr, sizeof(int*));

    printf("  double dval = 3.14159;\n");
    print_hexdump(&dval, sizeof(double));

    printf("  char str[] = \"Hello\";\n");
    print_hexdump(str, strlen(str) + 1); // +1 for null terminator
}

static void demo_struct_padding(void)
{
    printf("\n");
    printf("  Structure Padding Demonstration\n");
    printf("  --------------------------------\n");

    // Struct with padding
    struct example1 {
        char a;      // 1 byte
        // 7 bytes padding
        double b;    // 8 bytes
        char c;      // 1 byte
        // 7 bytes padding
    };

    // Optimized struct
    struct example2 {
        double b;    // 8 bytes
        char a;      // 1 byte
        char c;      // 1 byte
        // 6 bytes padding
    };

    printf("\n  struct example1 (with padding):\n");
    printf("  {\n");
    printf("      char a;      // 1 byte\n");
    printf("      double b;    // 8 bytes\n");
    printf("      char c;      // 1 byte\n");
    printf("  }\n");
    printf("  sizeof: %zu bytes (expected 10, got %zu due to padding)\n",
           sizeof(struct example1), sizeof(struct example1));
    printf("  sizeof(double): %zu bytes\n", sizeof(double));

    printf("\n");
    printf("  struct example2 (optimized):\n");
    printf("  {\n");
    printf("      double b;    // 8 bytes\n");
    printf("      char a;      // 1 byte\n");
    printf("      char c;      // 1 byte\n");
    printf("  }\n");
    printf("  sizeof: %zu bytes\n", sizeof(struct example2));

    struct example1 ex1;
    ex1.a = 'A';
    ex1.b = 3.14;
    ex1.c = 'B';

    printf("\n  Memory layout of example1:\n");
    printf("  -------------------------\n");
    printf("  ex1.a = 'A' (0x41)\n");
    printf("  ex1.b = 3.14\n");
    printf("  ex1.c = 'B' (0x42)\n\n");
    print_hexdump(&ex1, sizeof(struct example1));

    printf("  Observation:\n");
    printf("  - Bytes 1-7: Padding between 'a' and 'b'\n");
    printf("  - Bytes 17-23: Padding after 'c'\n");
    printf("  - Compiler adds padding for alignment\n");
}

static void demo_string_memory(void)
{
    printf("\n");
    printf("  String Memory Demonstration\n");
    printf("  -----------------------------\n");

    char str1[] = "Hello";
    char* str2 = "World";

    printf("\n  char str1[] = \"Hello\";\n");
    printf("  (Stack-allocated, mutable)\n");
    print_hexdump(str1, sizeof(str1));

    printf("  char* str2 = \"World\";\n");
    printf("  (Pointer to string literal, immutable)\n");
    printf("  Address of str2: %p\n", (void*)&str2);
    printf("  Value of str2 (points to): %p\n", (void*)str2);
    print_hexdump(str2, 6);

    printf("  Key difference:\n");
    printf("  - str1 is an array (5 chars + null = 6 bytes on stack)\n");
    printf("  - str2 is a pointer (8 bytes on stack, points to .rodata)\n");
}

static void demo_dynamic_allocation(void)
{
    printf("\n");
    printf("  Dynamic Memory Allocation\n");
    printf("  ---------------------------\n");

    int* arr = (int*)malloc(5 * sizeof(int));
    if (arr == NULL)
    {
        printf("  [Memory allocation failed]\n");
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        arr[i] = (i + 1) * 100;
    }

    printf("\n  int* arr = malloc(5 * sizeof(int));\n");
    printf("  Allocated memory address: %p\n", (void*)arr);
    printf("  Contents: {100, 200, 300, 400, 500}\n\n");

    print_hexdump(arr, 5 * sizeof(int));

    printf("  sizeof(int): %zu bytes\n", sizeof(int));
    printf("  Total allocated: %zu bytes\n", 5 * sizeof(int));

    free(arr);

    printf("\n  After free(), memory is returned but content may remain\n");
    printf("  (use-after-free is undefined behavior!)\n");
}

static void demo_memory_inspector_interactive(void)
{
    printf("\n");
    printf("  Interactive Memory Inspector\n");
    printf("  -----------------------------\n");

    int choice;
    printf("\n  Select demo type:\n");
    printf("  1. Integer in memory\n");
    printf("  2. String in memory\n");
    printf("  3. Structure with padding\n");
    printf("  4. Float/double representation\n");
    printf("  5. Pointer addresses\n");

    int status = safe_input_int(&choice, "\n  Enter choice: ", 1, 5);
    if (status != 0)
    {
        return;
    }

    printf("\n");

    switch (choice)
    {
        case 1:
        {
            int num = 0x41424344; // "ABCD" in hex
            printf("  int num = 0x41424344; // 'ABCD' as int\n");
            print_hexdump(&num, sizeof(int));
            print_memory_as_types(&num, sizeof(int));
            break;
        }
        case 2:
        {
            char str[] = "Memory";
            printf("  char str[] = \"Memory\";\n");
            print_hexdump(str, sizeof(str));
            break;
        }
        case 3:
        {
            struct { char a; int b; char c; } s;
            s.a = 'X';
            s.b = 12345;
            s.c = 'Y';
            printf("  struct { char a; int b; char c; } s;\n");
            printf("  sizeof: %zu bytes\n", sizeof(s));
            print_hexdump(&s, sizeof(s));
            break;
        }
        case 4:
        {
            float f = 3.14159f;
            double d = 3.14159265358979;
            printf("  float f = 3.14159f;\n");
            print_hexdump(&f, sizeof(float));
            printf("  double d = 3.14159265358979;\n");
            print_hexdump(&d, sizeof(double));
            break;
        }
        case 5:
        {
            int x = 42;
            int* p = &x;
            void* vp = (void*)p;
            printf("  int x = 42;\n");
            printf("  int* p = &x;\n");
            printf("  Address of x: %p\n", (void*)&x);
            printf("  Value of p: %p\n", (void*)p);
            print_hexdump(&p, sizeof(int*));
            print_hexdump(&vp, sizeof(void*));
            break;
        }
    }
}

void memory_inspector_demo(void)
{
    while (1)
    {
        display_header("Memory Inspector / Hexdump Visualizer");

        printf("\nExplore how data is stored in memory.\n");
        printf("Understand pointer layouts, byte padding, and data alignment.\n\n");

        printf("1. Basic hexdump demonstration\n");
        printf("2. Pointer sizes and addresses\n");
        printf("3. Structure padding analysis\n");
        printf("4. String memory layout\n");
        printf("5. Dynamic allocation (malloc/free)\n");
        printf("6. Interactive memory inspector\n");
        printf("7. Back to main menu\n");
        printf("\n");

        int choice;
        int status = safe_input_int(&choice, "Enter choice: ", 1, 7);
        if (status != 0)
        {
            break;
        }

        switch (choice)
        {
            case 1:
                demo_basic_hexdump();
                break;
            case 2:
                demo_pointer_sizes();
                break;
            case 3:
                demo_struct_padding();
                break;
            case 4:
                demo_string_memory();
                break;
            case 5:
                demo_dynamic_allocation();
                break;
            case 6:
                demo_memory_inspector_interactive();
                break;
            case 7:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}