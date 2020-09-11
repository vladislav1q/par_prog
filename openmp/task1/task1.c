#include "omp.h"
#include <stdio.h>

int main(int argc, char** argv) {

    #pragma omp parallel
    {
        printf("My id: %d\n", omp_get_thread_num());
    }

    return 0;
}
