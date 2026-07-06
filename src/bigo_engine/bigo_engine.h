#ifndef BIGO_ENGINE_H
#define BIGO_ENGINE_H

#include <stddef.h>

typedef enum {
    BIGO_CONSTANT,       // O(1)
    BIGO_LOGARITHMIC,    // O(log n)
    BIGO_LINEAR,         // O(n)
    BIGO_LINEARITHMIC,   // O(n log n)
    BIGO_QUADRATIC,      // O(n²)
    BIGO_CUBIC,          // O(n³)
    BIGO_EXPONENTIAL,    // O(2^n)
    BIGO_UNKNOWN
} BigOComplexity;

typedef enum {
    BIGO_HIGH_CONFIDENCE,
    BIGO_MEDIUM_CONFIDENCE,
    BIGO_LOW_CONFIDENCE,
    BIGO_NO_CONFIDENCE
} BigOConfidence;

typedef struct {
    BigOComplexity complexity;
    BigOConfidence confidence;
    double measured_ratio;
    double expected_ratio;
    double deviation_percent;
    size_t base_size;
    double base_time;
    double double_time;
} BigOResult;

typedef struct {
    const char* name;
    BigOComplexity expected;
} BigOComparison;

void bigo_engine_demo(void);

BigOResult analyze_algorithm_complexity(void (*sort_func)(int*, size_t), 
                                        int* arr, size_t size);

void print_bigo_result(const char* algorithm_name, BigOResult* result);

const char* complexity_to_string(BigOComplexity c);

const char* confidence_to_string(BigOConfidence c);

#endif // BIGO_ENGINE_H