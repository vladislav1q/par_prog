#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double ** matrix_create(int size);
void matrixDestroy(void** matrix, int size);
//sequent
void matrix_mult(double** A, double** B, double** C, int size);
//static
void matrix_mult_s(double** A, double** B, double** C, int size);
//dynamic
void matrix_mult_d(double** A, double** B, double** C, int size);
//guided
void matrix_mult_g(double** A, double** B, double** C, int size);

int main() {
    srand(time(NULL));

    int size = 1000;
    double end, start, interval;
    int num_tests = 2;

    double** A = matrix_create(size);
    double** B = matrix_create(size);
    double** C = matrix_create(size);

    FILE *file;
    file = fopen("results.txt","w");


    for(int n_threads = 1; n_threads <= 10; n_threads++){
        omp_set_num_threads(n_threads);
        printf("Number of threads: %d\n", n_threads);

        // Test sequent
        start = omp_get_wtime();
        for(int i = 0; i < num_tests; i++){
            matrix_mult(A, B, C, size);
        }
        end = omp_get_wtime();
        interval = end - start;
        printf("seq time execution for %d times: %f\n", num_tests, interval);
        fprintf(file,"%f ",interval);

        // Test static
        start = omp_get_wtime();
        for(int i = 0; i < num_tests; i++){
            matrix_mult_s(A, B, C, size);
        }
        end = omp_get_wtime();
        interval = end - start;
        printf("static time execution for %d times: %f\n", num_tests, interval);
        fprintf(file,"%f ",interval);

        // Test dynamic
        start = omp_get_wtime();
        for(int i = 0; i < num_tests; i++){
            matrix_mult_d(A, B, C, size);
        }
        end = omp_get_wtime();
        interval = end - start;
        printf("dynamic time execution for %d times: %f\n", num_tests, interval);
        fprintf(file,"%f ",interval);

        // Test guided
        start = omp_get_wtime();
        for(int i = 0; i < num_tests; i++){
            matrix_mult_g(A, B, C, size);
        }
        end = omp_get_wtime();
        interval = end - start;
        printf("guided time execution for %d times: %f\n", num_tests, interval);
        fprintf(file,"%f ",interval);
    }

    matrixDestroy((void**) A, size);
    matrixDestroy((void**) B, size);
    matrixDestroy((void**) C, size);

    fclose(file);

    return 0;
}

double** matrix_create(int size){
    double** A = malloc(size*sizeof(double*));
    for (int i = 0; i < size; i++) {
        A[i] = (double*)malloc(size*sizeof(double*));
        for (int j = 0; j < size; ++j)
            A[i][j] = (float)rand()/(float)(RAND_MAX);
    }
    return A;
}

void matrixDestroy(void ** matrix, int size){
    for(int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}

void matrix_mult(double** A, double** B, double** C, int size){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}

void matrix_mult_s(double** A, double** B, double** C, int size){
    #pragma omp parallel shared(A, B, C, size) default (none)
    {
        int n,m, sq_size = size*size;

        #pragma omp  for schedule(static)
        for (int i = 0; i < sq_size; ++i) {
            n = i / size;
            m = i % size;
            C[n][m] = 0;
            for (int k = 0; k < size; ++k)
                C[n][m] += A[n][k] * B[k][m];
        }
    #pragma omp barrier
    }
}

void matrix_mult_d(double** A, double** B, double** C, int size){
    #pragma omp parallel shared(A, B, C, size) default (none)
    {
        int n,m, sq_size = size*size;

        #pragma omp  for schedule(dynamic)
        for (int i = 0; i < sq_size; ++i) {
            n = i / size;
            m = i % size;
            C[n][m] = 0;
            for (int k = 0; k < size; ++k)
                C[n][m] += A[n][k] * B[k][m];
        }
    #pragma omp barrier
    }
}

void matrix_mult_g(double** A, double** B, double** C, int size){
    #pragma omp parallel shared(A, B, C, size) default (none)
    {
        int n,m, sq_size = size*size;

        #pragma omp  for schedule(guided)
        for (int i = 0; i < sq_size; ++i) {
            n = i / size;
            m = i % size;
            C[n][m] = 0;
            for (int k = 0; k < size; ++k)
                C[n][m] += A[n][k] * B[k][m];
        }
    #pragma omp barrier
    }
}
