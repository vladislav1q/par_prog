/******************************************************************************
* ЗАДАНИЕ: bugged4.c
* ОПИСАНИЕ:
*   Очень простая программа с segmentation fault.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

int main (int argc, char **argv)
{
    int nthreads, tid, i, j;
    // 2 problems:
    // N*N is a large chunk of memory to allocate statically
    // creating a copy of a in private(a) only creates a copy of pointer,
    // not allocating memory
//    double a[N][N];
    #pragma omp parallel shared(nthreads) private(i, j, tid)
    {
        // allocate memory for each thread
        double **a = malloc(N*sizeof(double*));
        for(int i = 0; i < N; i++){
            a[i] = malloc(N*sizeof(double));
        }

        tid = omp_get_thread_num();
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                a[i][j] = tid + i + j;

        printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);

        //free memory for each thread
        for(int i = 0; i < N; i++){
            free(a[i]);
        }
        free(a);
    }
}

