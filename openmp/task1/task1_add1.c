#include "omp.h"
#include <stdio.h>

int main(int argc, char** argv) {
    int my_id, max_id = 0;
    #pragma omp parallel private(my_id) shared(max_id)
    {
        my_id = omp_get_thread_num();
        #pragma omp single
        {
            max_id = omp_get_max_threads();
        };

        for( int i = max_id; i >= 0; i--){
            if(i == omp_get_thread_num()){
                printf("My id: %d\n", i);
            }
            #pragma omp barrier
        }
    }

    return 0;
}
