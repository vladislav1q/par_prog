#include <omp.h>
#include<stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n = 100;
    int i, nSum;

    #pragma omp parallel private(i) shared(nSum, n)
    {
        #pragma omp for
        for (i = 1; i <= n; i++) {
            #pragma omp atomic
            nSum += i;
        }
    }

    printf("Sum of %d natural numbers is: %d", n, nSum);

    return 0;
}
