#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define A 100
#define DEGREE 20
#define N 1048576
#define H 0.000019073486328125

void solve(long double* left_diag, long double* a, long double* right_diag, long double* b, long double* y);

int main(){
    // allocate memory
    long double* y_prev = calloc(N + 1, sizeof(long double));
    long double* y_next = calloc(N + 1, sizeof(long double));
    long double* a = calloc(N + 1, sizeof(long double));
    long double* b = calloc(N + 1, sizeof(long double));
    long double* left_diag = calloc(N + 1, sizeof(long double));
    long double* right_diag = calloc(N + 1, sizeof(long double));

    // set border values
    y_prev[0] = sqrt(2);
    y_prev[N] = sqrt(2);
    y_next[0] = sqrt(2);
    y_next[N] = sqrt(2);

    // initialize solution
    for (int i = 1; i < N; ++i){
        y_prev[i] = 0;
        y_next[i] = 0;
    }

    // open file for writing solution
    FILE *file;
    file = fopen("results.txt","w");

    for(int i = 0; i < 100; i++){
        //computations of matrix coefficients
        //right diagonal and left diagonal
        #pragma omp parallel for schedule(static)
        for (int k = 0; k < N + 1; k++){
            right_diag[k] = 1;
            left_diag[k] = 1;
        }
        //central diagonal
        #pragma omp parallel for schedule(static)
        for (int k = 1; k < N; k++)
            a[k] = A*H*H - 3*A*H*H*y_prev[k]*y_prev[k] - 2;

        //computations of right column of the system
        #pragma omp parallel for schedule(static)
        for (int k = 1; k < N; k++)
            b[k] = (A*H*H/12)*(y_prev[k+1]*y_prev[k+1]*y_prev[k+1] - 26*y_prev[k]*y_prev[k]*y_prev[k]
                    + y_prev[k-1]*y_prev[k-1]*y_prev[k-1] - y_prev[k+1] + 2*y_prev[k] - y_prev[k-1]);

        //solving the system of equations
        solve(left_diag, a, right_diag, b, y_next);
        //change pointers for prev and next solutions
        long double * buffer = y_prev;
        y_prev = y_next;
        y_next = buffer;
    }

    // print solution in file
    for(int i = 0; i < N+1; i++){
        fprintf(file,"%0.16Lf ",y_prev[i]);
    }
    // free memory and close files
    fclose(file);
    free(y_prev);
    free(y_next);
    free(right_diag);
    free(left_diag);
    free(a);
    free(b);

    return 0;
}

void solve(long double* left_diag, long double* a, long double* right_diag, long double* b, long double* y){
    // forward stroke

    //init start values
    int ind_start = 2, ind_end = N - 1, ind = 0;
    int step = 1;

    for (int i = 0; i < DEGREE; i++){
        ind_end = (ind_end-1) / 2;
        #pragma omp parallel for private(ind) shared(left_diag,a,right_diag,b,ind_start,ind_end,step)
        for (int k = 0; k < ind_end; k++) {
            //set current index
            ind = (k+1) * ind_start;
            //add new coefs to buffer
            long double buf1 = -left_diag[ind] * left_diag[ind-step] / a[ind-step];
            long double buf2 = - right_diag[ind] * right_diag[ind+step] / a[ind+step];
            long double buf3 = - right_diag[ind] * left_diag[ind+step] / a[ind+step]
                               - right_diag[ind-step] * left_diag[ind] / a[ind-step] + a[ind];
            long double buf4 = - right_diag[ind] * b[ind+step] / a[ind-step]
                               - left_diag[ind] * b[ind-step] / a[ind-step] + b[ind];
            //update coefs
            left_diag[ind] = buf1;
            right_diag[ind] = buf2;
            a[ind] = buf3;
            b[ind] = buf4;
        }
        ind_start *= 2;
        step *= 2;
    }

    // return stroke

    //init start values
    ind_end = 1;
    ind_start = N / 2;
    step = ind_start;

    for (int i = DEGREE-1; i > -1 ; i--){
        #pragma omp parallel for private(ind) shared(left_diag,a,right_diag,b,ind_start,ind_end,step,y)
        for (int k = 0; k < ind_end; k++) {
            //set current index
            ind = (1+2*k) * ind_start;
            //update solution
            y[ind] = (- left_diag[ind]*y[ind-step] - right_diag[ind]*y[ind+step] + b[ind])/a[ind];
        }
        ind_end *= 2;
        ind_start /= 2;
        step = ind_start;
    }

    return;
}
