#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int n = 10;

    long int a[n];
    long int b[n];
    int i;

    for(int i = 0; i < n; i++){
        a[i] = i+1;
    }

    #pragma omp parallel private(i) shared(a, b)
    {
        #pragma omp for
        for (i = 1; i < n-1; i++) {
            b[i-1] = (a[i-1] * a[i] * a[i+1]) / 3.0;
        }
    }
    printf("initial array = ");
    for (int i = 0; i < n; i++) {
        printf("%ld ", a[i]);
    }
    printf("\n");

    printf("resulting array = ");
    for (int i = 0; i < n-2; i++) {
        printf("%ld ", b[i]);
    }
    printf("\n");

    return 0;
}
