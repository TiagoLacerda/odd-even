#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

void swap(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

double *random_double_array(int length, double min, double max)
{
    double range = max - min, *array = (double *)malloc(sizeof(double) * length);

    for (int i = 0; i < length; i++)
    {
        array[i] = min + (double)(rand() * (range / RAND_MAX));
    }

    return array;
}

void read_double_array(double **array, int *length, char *path)
{
    FILE *file = fopen(path, "r");

    fscanf(file, "%d", length);

    *array = (double *)malloc(*length * sizeof(double));
    
    for (int i = 0; i < *length; i++)
    {
        fscanf(file, "%lf", &(*array)[i]);
    }

    fclose(file);
}

void save_double_array(double *array, int length, char *path)
{
    FILE *file = fopen(path, "w");

    fprintf(file, "%d\n", length);
    for (int i = 0; i < length; i++)
    {
        fprintf(file, "%.15lf\n", array[i]);
    }

    fclose(file);
}

void print_double_array(double *array, int length)
{
    if (length < 1)
    {
        printf("[]\n");
    }
    else
    {
        printf("[");

        for (int i = 0; i < length - 1; i++)
        {
            printf("%lf, ", array[i]);
        }

        printf("%lf]\n", array[length - 1]);
    }
}

/// @brief Returns index of first element out of order in [array]. Returns -1 if [array] is sorted.
int sorted(double *array, int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        if (array[i + 1] < array[i])
        {
            return i;
        }
    }
    return -1;
}
