#include <omp.h>
#include<stdio.h>
#include <stdlib.h>

long int fact(int n);
long double my_pow(double x, int n);

int main(int argc, char** argv) {
    int N = 128; // M,N must be even-numbered
    int M = 128; //

    double T_up = 0;
    double T_down = 100;
    double T_left = 100;
    double T_right = 100;

    FILE *file;
    file = fopen("results.txt","w");

    //vars for results and computations
    double tmp[M];
    double tmp2[N];
    double temp[M][N];

    // set all temp into zero
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++)
            temp[i][j] = 0;
    }
    // set border values
    for(int i = 0; i < M; i++){
        temp[i][0] = T_up;
        temp[i][N-1] = T_down;
    }
    for(int i = 0; i < N; i++){
        temp[0][i] = T_left;
        temp[M-1][i] = T_right;
    }

    for(int kkk = 0; kkk < 1000; kkk++) {
        #pragma omp parallel shared(temp,tmp)
        {
            //go from down to up border
            //each point is computed as the average among 4 neighbours
            for (int j = N-2; j > 0; j--) {
                #pragma omp for
                for (int i = 1; i < M - 1; i++) {
                    //put result in temporal variable
                    tmp[i - 1] = (temp[i - 1][j] + temp[i][j - 1] + temp[i + 1][j] + temp[i][j + 1]) / 4.0;
                }
                #pragma omp barrier
                #pragma omp master
                {
                    for (int i = 0; i < M - 2; i++)
                        temp[i + 1][j] = tmp[i];
                }
                #pragma omp barrier
            }
            //go from left to right border
            //each point is computed as the average among 4 neighbours
            for (int i = 1; i < M-1; i++) {
                #pragma omp for
                for (int j = 1; j < N - 1; j++) {
                    //put result in temporal variable
                    tmp2[j - 1] = (temp[i - 1][j] + temp[i][j - 1] + temp[i + 1][j] + temp[i][j + 1]) / 4.0;
                }
                #pragma omp barrier
                #pragma omp master
                {
                    for (int j = 0; j < N - 2; j++)
                        temp[i][j+1] = tmp2[j];
                }
                #pragma omp barrier
            }
            //go from right to left border
            //each point is computed as the average among 4 neighbours
            for (int i = M-2; i >0; i--) {
                #pragma omp for
                for (int j = 1; j < N - 1; j++) {
                    //put result in temporal variable
                    tmp2[j - 1] = (temp[i - 1][j] + temp[i][j - 1] + temp[i + 1][j] + temp[i][j + 1]) / 4.0;
                }
                #pragma omp barrier
                #pragma omp master
                {
                    for (int j = 0; j < N - 2; j++)
                        temp[i][j+1] = tmp2[j];
                }
                #pragma omp barrier
            }
            //go from up to down border
            //each point is computed as the average among 4 neighbours
            for (int j = 1; j < N-1; j++) {
                #pragma omp for
                for (int i = 1; i < M - 1; i++) {
                    //put result in temporal variable
                    tmp[i - 1] = (temp[i - 1][j] + temp[i][j - 1] + temp[i + 1][j] + temp[i][j + 1]) / 4.0;
                }
                #pragma omp barrier
                #pragma omp master
                {
                    for (int i = 0; i < M - 2; i++)
                        temp[i + 1][j] = tmp[i];
                }
                #pragma omp barrier
            }
        }
    }

    // print results for future processing
    for(int j = 0; j < N; j++){
        for(int i = 0; i < M; i++){
            fprintf(file,"%lf ",temp[i][j]);
        }
    }

    return 0;
}
