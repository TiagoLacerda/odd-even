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

#ifndef MPI_H
#define MPI_H
#include "mpi.h"
#endif

#ifndef UTIL_H
#define UTIL_H
#include "../lib/util.h"
#endif

#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "../lib/quicksort.h"
#endif

// Trade messages between two processes, exchanging chunks, with each keeping an end of the sorted result.
void compare_split(double *chunk, int chunk_length, int rank, int lo_rank, int hi_rank)
{
    MPI_Status status;
    double other[chunk_length];

    // Trade messages between processes
    MPI_Sendrecv(chunk, chunk_length, MPI_DOUBLE, (rank == lo_rank) ? hi_rank : lo_rank, 0, other, chunk_length, MPI_DOUBLE, (rank == lo_rank) ? hi_rank : lo_rank, 0, MPI_COMM_WORLD, &status);

    // Sort chunk data
    if (rank == lo_rank)
    {
        quicksort_split(chunk, other, chunk_length, chunk_length);
    }
    else
    {
        quicksort_split(other, chunk, chunk_length, chunk_length);
    }
}

/// @brief Parallel odd-even sort.
/// @return Elapsed wall time in seconds.
double odd_even_sort(double **array, int array_length, int size, int rank)
{
    int chunk_length;
    double *chunk;

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    if (rank == 0)
    {
        // Define chunk length
        chunk_length = array_length / size + (array_length % size != 0);

        // Reallocate memory for array to fit padded infinity.
        *array = (double *)realloc(*array, chunk_length * size * sizeof(double));

        // Add "infinity" to end of [array], if it is larger than there are numbers read from file
        for (int i = array_length; i < chunk_length * size; i++)
        {
            (*array)[i] = __INT_MAX__;
        }
    }

    // Broadcast chunk length
    MPI_Bcast(&chunk_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory to receive chunks
    chunk = (double *)malloc(chunk_length * sizeof(double));

    // Scatter array into chunks between processes
    MPI_Scatter(*array, chunk_length, MPI_DOUBLE, chunk, chunk_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Free memory
    if (rank == 0)
    {
        free(*array);
    }

    // Sort chunk
    quicksort(chunk, chunk_length);

    // Perform odd-even transposition
    for (int i = 0; i < size; i++)
    {
        // ODD ITERATION
        if (i % 2 == 0)
        {
            if ((rank % 2 == 0) && (rank < size - 1))
            {
                compare_split(chunk, chunk_length, rank, rank, rank + 1);
            }
            else if (rank % 2 == 1)
            {
                compare_split(chunk, chunk_length, rank, rank - 1, rank);
            }
        }
        // EVEN ITERATION
        else
        {
            if ((rank % 2 == 1) && (rank < size - 1))
            {
                compare_split(chunk, chunk_length, rank, rank, rank + 1);
            }
            else if ((rank % 2 == 0) && (rank > 0))
            {
                compare_split(chunk, chunk_length, rank, rank - 1, rank);
            }
        }
    }

    // Allocate memory for array
    if (rank == 0)
    {
        *array = (double *)malloc(chunk_length * size * sizeof(double));
    }

    // Merge sorted chunks into array
    MPI_Gather(chunk, chunk_length, MPI_DOUBLE, *array, chunk_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Free memory
    free(chunk);

    clock_gettime(CLOCK_REALTIME, &end);

    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds * 1e-9;
    return elapsed;
}

int validate_args(int argc, char **argv, int rank)
{
    if (argc < 3)
    {
        if (rank == 0)
        {
            printf("\033[31mNot enough arguments! Please provide both a path to a file containing an array and a path to a file where results will be written.\033[0m\n");
        }
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    int size;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!validate_args(argc, argv, rank))
    {
        MPI_Finalize();
        return 0;
    }

    int length;
    double *array;
    char *src = argv[1];
    char *dst = argv[2];

    if (rank == 0)
    {
        read_double_array(&array, &length, src);
    }

    double elapsed = odd_even_sort(&array, length, size, rank);

    if (rank == 0)
    {
        int index = sorted(array, length);
        if (index != -1)
        {
            printf("\033[31m(MPI) Result array was not sorted! Inconsistency at index %d\033[0m\n", index);
        }

        FILE *file = fopen(dst, "a");
        fprintf(file, "%d, %d, %.15lf,\n", size, length, elapsed);
        fclose(file);
    }

    MPI_Finalize();
    return 0;
}
