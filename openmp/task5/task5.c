#include <omp.h>
#include<stdio.h>
#include <stdlib.h>

long int fact(int n);
long double my_pow(double x, int n);

int main(int argc, char** argv) {
    int n_elements = atoi(argv[1]);
    long double Sum = 0;
    double x = 0.2;

    #pragma omp parallel shared(n_elements,x)
    {
        int id = omp_get_thread_num();
        int n_threads = omp_get_max_threads();
        long double* results = (long double*)malloc(n_threads*sizeof(long double));
        results[id] = 0;

        double start = omp_get_wtime();

        #pragma omp for schedule(static)
        for (int i = 0; i <= n_elements; i++) {
            results[id] += my_pow(x,i)/fact(i);
        }

        if(id == 0){
            for (int i = 0; i < n_threads; i++) {
                Sum += results[i];
            }
            double end = omp_get_wtime();
            double interval = end - start;
            printf("time execution for %d threads: %f\n", n_threads, interval);

            free(results);
        };
    }

    printf("Sum of %d elements of exp(%.2lf) taylor series: %Lf\n", n_elements, x, Sum);

    return 0;
}

long int fact(int n){
    if (n <= 1)
        return 1;
    else
        return n*fact(n-1);
};

long double my_pow(double x, int n){
    if(n == 0)
        return 1;
    int i;
    long double res = x;
    for(i = 1; i < n; i++){
        res = res*x;
    }
    return res;
}
