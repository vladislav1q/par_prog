#include <omp.h>
#include<stdio.h>
#include <stdlib.h>
#include <math.h>

long int fact(int n);
long double my_pow(double x, int n);

//This function calculates taylor series of erf(x)
//Input argument is the number of elements in sum

int main(int argc, char** argv) {
    int n_elements = atoi(argv[1]);
    long double Sum = 0;
    double x = 0.5;

    #pragma omp parallel shared(n_elements,x)
    {
        int id = omp_get_thread_num();
        int n_threads = omp_get_max_threads();
        long double* results = (long double*)malloc(n_threads*sizeof(long double));
        results[id] = 0;

        double start = omp_get_wtime();

        //calculation of full sum is divided into calculating
        //each range of elements independently
        #pragma omp for schedule(static)
        for (int i = 0; i <= n_elements; i++) {
            results[id] += 2*my_pow(x,2*i+1)*my_pow(-1,i)/fact(i)/(2*i+1)/sqrt(M_PI);
        }

        //On the main process we combine all sums
        //resulted from parallel execution
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

    printf("Sum of %d elements of erf(%.2lf) taylor series: %Lf\n", n_elements, x, Sum);

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
