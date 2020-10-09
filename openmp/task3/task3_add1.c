#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void print_array(long int *a, int n){
    for (int i = 0; i < n; i++) {
        printf("%ld ", a[i]);
    }
    printf("\n");
    return;
}

int main(int argc, char** argv) {
    int n = 24;
    long int a[n];
    long int buff;
    int check;

    for(int i = 0; i < n; i++){
        a[i] = i+1;
    }

    printf("initial array = ");
    print_array(a,n);

    omp_lock_t simple_lock;
    omp_init_lock(&simple_lock);

    buff = a[n-1];

    #pragma omp parallel shared(a, simple_lock, check,n) default (none)
    {
        // buffers for computations
        long int buffer1, buffer2, buffer3;
        int iter_first = 2;

        int num_threads = omp_get_num_threads();
        int id = omp_get_thread_num();

        #pragma omp master
        {
            check = num_threads - 1;
        }

        #pragma omp  for schedule(static)
        for (int i = 2; i < n; i++) {
            if (iter_first) {
                while (1) {
                    // synchronization of two first cycles
                    if (id == check) {
                        omp_set_lock(&simple_lock);
                        if (iter_first == 2) {
                            buffer3 = a[i - 2];
                            buffer2 = a[i - 1];
                            buffer1 = a[i];
                            iter_first--;

                            a[i] = (buffer3 * buffer2 * buffer1) / 3;
                        } else {
                            buffer3 = buffer2;
                            buffer2 = buffer1;
                            buffer1 = a[i];
                            iter_first--;

                            a[i] = (buffer3 * buffer2 * buffer1) / 3;
                        }

                        if(check != 0){
                            check--;
                        } else {
                            check = num_threads - 1;
                        }

                        omp_unset_lock(&simple_lock);
                        break;
                    }
                }
            } else {
                buffer3 = buffer2;
                buffer2 = buffer1;
                buffer1 = a[i];

                a[i] = (buffer3 * buffer2 * buffer1) / 3;
            }
        }
    }

    for (int i = 1; i < n-1; ++i) {
        a[i] = a[i+1];
    }
    a[n-1] = buff;

    printf("resulting array = ");
    print_array(a,n);

    omp_destroy_lock(&simple_lock);

    return 0;
}


