#define _GNU_SOURCE
#include "../src/compression/compression.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void generate_repetitive(char* str, int len)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = 'A' + ((i / 50) % 2);
    }
    str[len] = '\0';
}

static void generate_random_chars(char* str, int len)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = 'A' + (rand() % 26);
    }
    str[len] = '\0';
}

static void generate_structured(char* str, int len)
{
    const char* word = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG ";
    int word_len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        str[i] = word[i % word_len];
    }
    str[len] = '\0';
}

static void run_rle(const char* input, int len, int* comp_bytes)
{
    char* out = malloc(len * 2 + 100);
    if (!out)
        return;
    int out_len = rle_encode(input, out, len, len * 2 + 100);
    *comp_bytes = out_len;
    char* dec = malloc(len + 100);
    if (dec)
    {
        rle_decode(out, out_len, dec, len + 100);
        free(dec);
    }
    free(out);
}

static void run_huffman(const char* input, int len, int* comp_bytes)
{
    HuffmanNode* root = build_huffman_tree(input);
    if (!root)
        return;
    char codes[256][256];
    memset(codes, 0, sizeof(codes));
    char current_code[256];
    generate_huffman_codes(root, codes, current_code, 0);
    char* out = malloc(len * 8 + 100);
    if (out)
    {
        int bits = huffman_encode(input, codes, out, len * 8 + 100);
        *comp_bytes = (bits + 7) / 8;
        char* dec = malloc(len + 100);
        if (dec)
        {
            huffman_decode(out, root, dec, len + 100);
            free(dec);
        }
        free(out);
    }
    free_huffman_tree(root);
}

static void run_lzw(const char* input, int len, int* comp_bytes)
{
    int* out = malloc(sizeof(int) * (len * 2 + 100));
    if (!out)
        return;
    int codes = lzw_encode(input, out, len * 2 + 100);
    *comp_bytes = (codes * 12 + 7) / 8;
    char* dec = malloc(len + 100);
    if (dec)
    {
        lzw_decode(out, codes, dec, len + 100);
        free(dec);
    }
    free(out);
}

static void run_bwt_mtf(const char* input, int len, int* comp_bytes)
{
    char* bwt_out = malloc(len + 100);
    if (!bwt_out)
        return;
    int primary_index = 0;
    int bwt_len = bwt_forward(input, bwt_out, &primary_index);

    char* mtf_out = malloc(len + 100);
    if (mtf_out)
    {
        int mtf_len = mtf_encode(bwt_out, mtf_out, bwt_len);
        char* rle_out = malloc(len * 2 + 100);
        if (rle_out)
        {
            int rle_len = rle_encode(mtf_out, rle_out, mtf_len, len * 2 + 100);
            *comp_bytes = rle_len;

            char* rle_dec = malloc(len + 100);
            if (rle_dec)
            {
                int rle_dec_len = rle_decode(rle_out, rle_len, rle_dec, len + 100);
                char* mtf_dec = malloc(len + 100);
                if (mtf_dec)
                {
                    mtf_decode(rle_dec, mtf_dec, rle_dec_len);
                    char* bwt_dec = malloc(len + 100);
                    if (bwt_dec)
                    {
                        bwt_inverse(mtf_dec, primary_index, bwt_dec);
                        free(bwt_dec);
                    }
                    free(mtf_dec);
                }
                free(rle_dec);
            }
            free(rle_out);
        }
        free(mtf_out);
    }
    free(bwt_out);
}

void run_compression_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    char* text = malloc(n + 1);
    if (!text)
    {
        return;
    }

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: COMPRESSION ALGORITHMS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-20s %-15s %-15s %-15s\n", "Algorithm", "Input Type", "Time (ms)", "Comp Ratio");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {"RLE", "Huffman", "LZW", "BWT+MTF+RLE"};
    const char* input_types[] = {"Repetitive", "Random", "Structured"};

    for (int a = 0; a < 4; a++)
    {
        // Skip BWT for large N to avoid slow sorting
        if (a == 3 && n > 20000)
        {
            for (int t = 0; t < 3; t++)
            {
                printf("%-20s %-15s %-15s %-15s\n", algos[a], input_types[t], "N/A", "BYPASSED");
            }
            continue;
        }

        for (int t = 0; t < 3; t++)
        {
            switch (t)
            {
                case 0:
                    generate_repetitive(text, n);
                    break;
                case 1:
                    generate_random_chars(text, n);
                    break;
                case 2:
                    generate_structured(text, n);
                    break;
            }

            size_t mem_before = benchmark_get_peak_memory();
            double start_time = benchmark_get_time();

            int comp_bytes = 0;
            switch (a)
            {
                case 0:
                    run_rle(text, n, &comp_bytes);
                    break;
                case 1:
                    run_huffman(text, n, &comp_bytes);
                    break;
                case 2:
                    run_lzw(text, n, &comp_bytes);
                    break;
                case 3:
                    run_bwt_mtf(text, n, &comp_bytes);
                    break;
            }

            double duration = benchmark_get_time() - start_time;
            size_t mem_after = benchmark_get_peak_memory();
            size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
            if (mem_used == 0)
                mem_used = mem_after;

            double ratio = (1.0 - (double)comp_bytes / n) * 100.0;

            char ratio_str[32];
            snprintf(ratio_str, sizeof(ratio_str), "%.2f%%", ratio);

            printf("%-20s %-15s %-15.6f %-15s\n", algos[a], input_types[t], duration * 1000.0,
                   ratio_str);

            char csv_algo_name[64];
            snprintf(csv_algo_name, sizeof(csv_algo_name), "%s (%s)", algos[a], input_types[t]);
            benchmark_export_csv("compression", csv_algo_name, n, duration, mem_used);
        }
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/compression.csv'.\n");

    free(text);
}
