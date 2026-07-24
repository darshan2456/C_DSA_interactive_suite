#include "bigo_verifier.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* bigo_complexity_to_string(BigOComplexity complexity)
{
    switch (complexity)
    {
        case BIGO_O1:
            return "O(1)";
        case BIGO_OLOGN:
            return "O(log N)";
        case BIGO_ON:
            return "O(N)";
        case BIGO_ONLOGN:
            return "O(N log N)";
        case BIGO_ON2:
            return "O(N^2)";
        case BIGO_ON3:
            return "O(N^3)";
        default:
            return "O(Unknown)";
    }
}

BigOComplexity deduce_bigo_from_ratio(double ratio, int n)
{
    if (ratio <= 0.0)
    {
        return BIGO_UNKNOWN;
    }

    // Expected theoretical growth ratios T(2N) / T(N):
    // O(1)      => ~1.0
    // O(log N)  => (log(2N) / log(N)) = 1 + (log 2 / log N)  => ~1.1 to 1.3
    // O(N)      => ~2.0
    // O(N log N)=> 2 * (1 + log 2 / log N)                   => ~2.2 to 2.5
    // O(N^2)    => ~4.0
    // O(N^3)    => ~8.0

    double log_factor = (n > 1) ? (1.0 + (log(2.0) / log((double)n))) : 1.2;
    double expected_onlogn_ratio = 2.0 * log_factor;

    if (ratio < 1.15)
    {
        return BIGO_O1;
    }
    else if (ratio < 1.6)
    {
        return BIGO_OLOGN;
    }
    else if (ratio < (expected_onlogn_ratio - 0.15))
    {
        return BIGO_ON;
    }
    else if (ratio < 3.1)
    {
        return BIGO_ONLOGN;
    }
    else if (ratio < 6.0)
    {
        return BIGO_ON2;
    }
    else
    {
        return BIGO_ON3;
    }
}

static double measure_execution_time_ms(AlgFunction func, int n)
{
    if (func == NULL || n <= 0)
    {
        return 0.0;
    }

    int* data = (int*)malloc((size_t)n * sizeof(int));
    if (!data)
    {
        return 0.0;
    }

    // Populate with pseudorandom numbers
    srand(42);
    for (int i = 0; i < n; i++)
    {
        data[i] = rand() % (n * 10 + 1);
    }

    clock_t start = clock();
    func(data, n);
    clock_t end = clock();

    free(data);

    double duration_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
    return (duration_ms < 0.001) ? 0.001 : duration_ms;
}

bool run_bigo_profile(const char* alg_name, const char* theoretical_str, AlgFunction func,
                      int base_n, int samples_to_run, BigOReport* report)
{
    if (!alg_name || !func || !report || base_n <= 0)
    {
        return false;
    }

    memset(report, 0, sizeof(BigOReport));
    report->algorithm_name = alg_name;
    report->theoretical_complexity = theoretical_str ? theoretical_str : "Unknown";

    if (samples_to_run < 2)
    {
        samples_to_run = 2;
    }
    if (samples_to_run > MAX_BIGO_SAMPLES)
    {
        samples_to_run = MAX_BIGO_SAMPLES;
    }

    report->sample_count = samples_to_run;

    int current_n = base_n;
    for (int i = 0; i < samples_to_run; i++)
    {
        report->samples[i].n = current_n;

        // Run 3 trials and average to reduce hardware noise
        double t1 = measure_execution_time_ms(func, current_n);
        double t2 = measure_execution_time_ms(func, current_n);
        double t3 = measure_execution_time_ms(func, current_n);

        report->samples[i].time_ms = (t1 + t2 + t3) / 3.0;
        current_n *= 2;
    }

    report->ratio_count = samples_to_run - 1;
    double sum_ratio = 0.0;

    for (int i = 0; i < report->ratio_count; i++)
    {
        double prev_t = report->samples[i].time_ms;
        double next_t = report->samples[i + 1].time_ms;
        double ratio = (prev_t > 0.0) ? (next_t / prev_t) : 1.0;

        report->ratios[i].n_start = report->samples[i].n;
        report->ratios[i].n_end = report->samples[i + 1].n;
        report->ratios[i].ratio = ratio;
        report->ratios[i].deduced_class = deduce_bigo_from_ratio(ratio, report->samples[i].n);

        sum_ratio += ratio;
    }

    double avg_ratio = sum_ratio / (double)report->ratio_count;
    report->overall_empirical_class = deduce_bigo_from_ratio(avg_ratio, report->samples[0].n);
    report->overall_empirical_str = bigo_complexity_to_string(report->overall_empirical_class);

    report->matches_theoretical =
        (strstr(report->overall_empirical_str, report->theoretical_complexity) != NULL ||
         strstr(report->theoretical_complexity, report->overall_empirical_str) != NULL);

    return true;
}

void print_bigo_report(const BigOReport* report)
{
    if (!report)
    {
        printf("(NULL BigOReport pointer)\n");
        return;
    }

    printf("\n========================================================================\n");
    printf("        EMPIRICAL ASYMPTOTIC COMPLEXITY VERIFIER (BIG-O ENGINE)        \n");
    printf("========================================================================\n");
    printf("Algorithm: %-25s | Theoretical: %s\n", report->algorithm_name,
           report->theoretical_complexity);
    printf("------------------------------------------------------------------------\n");
    printf("  %-10s | %-16s | %-14s | %-12s\n", "Size (N)", "Time (ms)", "Ratio T(2N)/T(N)",
           "Deduced Bound");
    printf("  -----------+------------------+----------------+--------------\n");

    for (int i = 0; i < report->sample_count; i++)
    {
        if (i < report->ratio_count)
        {
            printf("  N=%-8d | %-16.3f | %-14.2fx | %-12s\n", report->samples[i].n,
                   report->samples[i].time_ms, report->ratios[i].ratio,
                   bigo_complexity_to_string(report->ratios[i].deduced_class));
        }
        else
        {
            printf("  N=%-8d | %-16.3f | %-14s | %-12s\n", report->samples[i].n,
                   report->samples[i].time_ms, "-", "-");
        }
    }

    printf("========================================================================\n");
    printf("Empirical Bound Deduced: %s\n", report->overall_empirical_str);
    printf("Theoretical Match: %s\n", report->matches_theoretical ? "[MATCHED]" : "[DIVERGED]");
    printf("========================================================================\n\n");
}
