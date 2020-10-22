/******************************************************************************
* ЗАДАНИЕ: bugged2.c
* ОПИСАНИЕ:
*   Еще одна программа на OpenMP с багом. 
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    float total;

//    #pragma omp parallel
//    Variables are not specified as shared or private
//    used default values
//
//    if you want to have sum ('total' variable) - unique for each thread
//    put it in private
//    else put it in shared and use omp atomic directive before 'total' update
    #pragma omp parallel shared(nthreads) private(tid, total) default(none)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d is starting...\n", tid);

        total = 0.0;
        // without barrier for shared 'total' -> undefined behaviour
//        #pragma omp barrier
        #pragma omp for schedule(dynamic, 10)
        for (i = 0; i < 1000000; i++)
            total = total + i*1.0;

        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
}

