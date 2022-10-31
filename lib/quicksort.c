#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "quicksort.h"
#endif

#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

int partition(double *array, int lo, int hi)
{
    int i = lo - 1, j = hi + 1, index = lo + (hi - lo) / 2;
    double pivot = array[index];

    while (1)
    {
        do
        {
            i++;
        } while (array[i] < pivot);

        do
        {
            j--;
        } while (array[j] > pivot);

        if (i >= j)
        {
            return j;
        }

        swap(&array[i], &array[j]);
    }
}

void quicksort_internal(double *array, int lo, int hi)
{
    if (lo >= 0 && hi >= 0 && lo < hi)
    {
        int p = partition(array, lo, hi);
        quicksort_internal(array, lo, p);
        quicksort_internal(array, p + 1, hi);
    }
}

void quicksort(double *array, int length)
{
    quicksort_internal(array, 0, length - 1);
}

double get_value(double *array1, double *array2, int length1, int length2, int index)
{
    if (index < length1)
    {
        return array1[index];
    }
    else
    {
        return array2[index - length1];
    }
}

void set_value(double *array1, double *array2, int length1, int length2, int index, double value)
{
    if (index < length1)
    {
        array1[index] = value;
    }
    else
    {
        array2[index - length1] = value;
    }
}

int partition_split(double *array1, double *array2, int length1, int length2, int lo, int hi)
{
    int i = lo - 1, j = hi + 1, index = lo + (hi - lo) / 2;
    double pivot = get_value(array1, array2, length1, length2, index);

    while (1)
    {
        do
        {
            i++;
        } while (get_value(array1, array2, length1, length2, i) < pivot);

        do
        {
            j--;
        } while (get_value(array1, array2, length1, length2, j) > pivot);

        if (i >= j)
        {
            return j;
        }

        double temp1 = get_value(array1, array2, length1, length2, i);
        double temp2 = get_value(array1, array2, length1, length2, j);

        set_value(array1, array2, length1, length2, i, temp2);
        set_value(array1, array2, length1, length2, j, temp1);
    }
}

void quicksort_internal_split(double *array1, double *array2, int length1, int length2, int lo, int hi)
{
    if (lo >= 0 && hi >= 0 && lo < hi)
    {
        int p = partition_split(array1, array2, length1, length2, lo, hi);
        quicksort_internal_split(array1, array2, length1, length2, lo, p);
        quicksort_internal_split(array1, array2, length1, length2, p + 1, hi);
    }
}

void quicksort_split(double *array1, double *array2, int length1, int length2)
{
    quicksort_internal_split(array1, array2, length1, length2, 0, length1 + length2 - 1);
}