#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef OMP_H
#define OMP_H
#include "omp.h"
#endif

#ifndef UTIL_H
#define UTIL_H
#include "../lib/util.h"
#endif

#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "../lib/quicksort.h"
#endif

void workload(int n, int k, int i, int *s, int *f)
{
    if (n <= 0 || k <= 0 || i < 0)
    {
        *s = -1;
        *f = -1;
    }

    *s = (n / k) * i + min((n % k), i);
    *f = *s + max(0, (n / k) - 1) + (n % k > i);
}

/// @brief Parallel odd-even sort.
/// @return Elapsed wall time in seconds.
double odd_even_sort(double **array, int array_length, int size)
{
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    // Divide array in 2K chunks and sort them
    // This is not necessary (therefore commented) because there is an even number of chunks, and the first iteration in odd-even transposition sorts every pair of chunks

    // #pragma omp parallel num_threads(size)
    // {
    //     int n = array_length, k = omp_get_num_threads(), i = omp_get_thread_num(), s, f;
    //     workload(n, 2 * k, 2 * i + 0, &s, &f);
    //     quicksort(&((*array)[s]), (f - s) + 1);

    //     workload(n, 2 * k, 2 * i + 1, &s, &f);
    //     quicksort(&((*array)[s]), (f - s) + 1);
    // }

    // Perform odd-even transposition
    for (int i = 0; i < 2 * size; i++)
    {
        // ODD ITERATION
        if (i % 2 == 0)
        {
            #pragma omp parallel num_threads(size)
            {
                int n = array_length, k = omp_get_num_threads(), id = omp_get_thread_num(), s1, s2, f1, f2;
                workload(n, 2 * k, 2 * id + 0, &s1, &f1);
                workload(n, 2 * k, 2 * id + 1, &s2, &f2);
                printf("%d, %d\n", s1, f2);
                quicksort(&((*array)[s1]), (f2 - s1) + 1);
            }
        }
        // EVEN ITERATION
        else
        {
            #pragma omp parallel num_threads(size)
            {
                int n = array_length, k = omp_get_num_threads(), id = omp_get_thread_num(), s1, s2, f1, f2;
                if (id < k - 1) // WASTING A THREAD
                {
                    workload(n, 2 * k, 2 * (id + 0) + 1, &s1, &f1);
                    workload(n, 2 * k, 2 * (id + 1) + 0, &s2, &f2);
                    quicksort(&((*array)[s1]), (f2 - s1) + 1);
                }
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);

    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds * 1e-9;
    return elapsed;
}

int validate_args(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    if (argc < 4)
    {
        printf("\033[31mNot enough arguments! Please provide the number of threads, a path to a file containing an array and a path to a file where results will be written.\033[0m\n");
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (!validate_args(argc, argv))
    {
        return 0;
    }

    int size = strtol(argv[1], NULL, 10);
    omp_set_num_threads(size);

    int length;
    double *array;
    char *src = argv[2];
    char *dst = argv[3];

    read_double_array(&array, &length, src);

    double elapsed = odd_even_sort(&array, length, size);

    int index = sorted(array, length);
    if (index != -1)
    {
        printf("\033[31m(OMP) Result array was not sorted! Inconsistency at index %d\033[0m\n", index);
    }

    FILE *file = fopen(dst, "a");
    fprintf(file, "%d, %d, %.15lf,\n", size, length, elapsed);
    fclose(file);

    return 0;
}
