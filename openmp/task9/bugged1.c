/******************************************************************************
* ЗАДАНИЕ: bugged1.c
* ОПИСАНИЕ:
*   Данная программа демонстрирует использование конструкции 'parallel for'.
*   Однако, данный код вызывает ошибки компиляции.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N           50
#define CHUNKSIZE   5

int main(int argc, char **argv)
{
    int i, chunk, tid;
    float a[N], b[N], c[N];

    for (i = 0; i < N; i++)
        a[i] = b[i] = i * 1.0;
    chunk = CHUNKSIZE;

//    #pragma omp parallel for    \
//        shared(a, b, c, chunk)  \
//        private(i, tid)         \
//        schedule(static, chunk)
//    {
//        tid = omp_get_thread_num();
//        for (i = 0; i < N; i++)
//        {
//            c[i] = a[i] + b[i];
//            printf("tid= %d i= %d c[i]= %f\n", tid, i, c[i]);
//        }
//    }
//    Incorrect use of omp parallel
//    use 'omp parallel{ omp for }'
//    or 'omp parallel for' without {}
    #pragma omp parallel for    \
    shared(a, b, c, chunk)  \
    private(i, tid)         \
    schedule(static, chunk)
    for (i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
        printf("tid= %d i= %d c[i]= %f\n", omp_get_thread_num(), i, c[i]);
    }
}
