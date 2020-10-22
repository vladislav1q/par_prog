/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

//create global variable
float sum = 0;

float dotprod()
{
    int i, tid;
    //here sum will be private(from main), but must be shared
    //that is why global sum was created
//    float sum;

    tid = omp_get_thread_num();

    //was
//    #pragma omp for reduction(+:sum)
    //add parallel
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < VECLEN; i++)
    {
        sum = sum + (a[i] * b[i]);
        printf("  tid= %d i=%d\n", tid, i);
    }
}


int main (int argc, char *argv[])
{
    int i;
    //global instead
//    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
//    sum = 0.0;

    //we will use parallel in dotprod()
//    #pragma omp parallel shared(sum)
    dotprod();

    printf("Sum = %f\n",sum);
}

