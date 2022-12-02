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

/// @brief Parallel odd-even sort.
/// @return Elapsed wall time in seconds.
double odd_even_sort(double **array, int array_length, int size)
{
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    int iteration;
    double temp;
    // Perform odd-even transposition
    #pragma omp parallel num_threads(size) shared(array, array_length) private(iteration, temp)
    {
        for (iteration = 0; iteration < array_length; iteration++)
        {
            // ODD ITERATION
            if (iteration % 2 == 0)
            {
                #pragma omp for
                for (int i = 1; i < array_length; i += 2)
                {
                    if ((*array)[i - 1] > (*array)[i])
                    {
                        temp = (*array)[i];
                        (*array)[i] = (*array)[i - 1];
                        (*array)[i - 1] = temp;
                    }
                }
            }
            // EVEN ITERATION
            else
            {
                #pragma omp for
                for (int i = 1; i < array_length - 1; i += 2)
                {
                    if ((*array)[i] > (*array)[i + 1])
                    {
                        temp = (*array)[i];
                        (*array)[i] = (*array)[i + 1];
                        (*array)[i + 1] = temp;
                    }
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
        printf("\033[31m(ALT) Result array was not sorted! Inconsistency at index %d\033[0m\n", index);
    }

    FILE *file = fopen(dst, "a");
    fprintf(file, "%d, %d, %.15lf,\n", size, length, elapsed);
    fclose(file);

    return 0;
}
