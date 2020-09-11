#include <omp.h>
#include<stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    int i, nSum = 0;

    #pragma omp parallel private(i) shared(nSum, n)
    {
        #pragma omp for private(i) reduction(+:nSum) schedule(static)
        for (i = 1; i <= n; i++) {
            nSum += i;
        }
    }

    printf("Sum of %d natural numbers is: %d", n, nSum);

    return 0;
}

