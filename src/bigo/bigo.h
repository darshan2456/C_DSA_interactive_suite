#ifndef BIGO_H
#define BIGO_H

#include <stddef.h>

typedef enum
{
    BIGO_O_1,
    BIGO_O_LOG_N,
    BIGO_O_N,
    BIGO_O_N_LOG_N,
    BIGO_O_N_SQUARED,
    BIGO_O_N_CUBED,
    BIGO_O_2_N,
    BIGO_UNKNOWN
} BigOComplexity;

typedef struct
{
    size_t input_size;
    double time_ms;
} BenchmarkResult;

typedef struct
{
    size_t n;
    double time_n;
    double time_2n;
    double time_4n;
    double ratio_2n_1n;
    double ratio_4n_2n;
    BigOComplexity deduced_complexity;
} BigOAnalysis;

void bigo_sorting_demo(void);
void bigo_searching_demo(void);
void bigo_analysis_demo(void);

#endif
