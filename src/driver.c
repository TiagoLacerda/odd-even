#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef UNISTD_H
#define UNISTD
#include <unistd.h>
#endif

#ifndef UTIL_H
#define UTIL_H
#include "../lib/util.h"
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef SYS_STAT_H
#define SYS_STAT_H
#include <sys/stat.h>
#endif

int main(int argc, char **argv)
{
    int samples = 10;
    double min = 0.0;
    double max = 1.0;

    char command[500], src[100], dst[100];

    srand(time(NULL));

    mkdir("data", 0777); // Assume success

    for (int length = 1000; length <= 40000; length += 1000)
    {
        sprintf(src, "./data/data_%d_%lf_%lf.txt", length, min, max);

        double *array = random_double_array(length, min, max);
        save_double_array(array, length, src);
        free(array);

        sprintf(dst, "./data/result_mpi.csv");
        for (int processes = 1; processes <= 8; processes *= 2)
        {
            for (int sample = 0; sample < samples; sample++)
            {
                sprintf(command, "mpirun -n %d --oversubscribe odd-even-open-mpi %s %s", processes, src, dst);
                system(command);
            }
        }

        sprintf(dst, "./data/result_mp.csv");
        for (int threads = 1; threads <= 8; threads *= 2)
        {
            for (int sample = 0; sample < samples; sample++)
            {
                sprintf(command, "./odd-even-open-mp %d %s %s", threads, src, dst);
                system(command);
            }
        }
    }

    return 0;
}