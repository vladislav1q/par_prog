#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define ISIZE 1000
#define JSIZE 1000

typedef struct {
    int start;
    int end;
} SEGMENT;

double master(int np){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //ALLOCATE MEMORY
    int block_size = ISIZE / np;
    SEGMENT* data;
    data = (SEGMENT*)calloc(np, sizeof(SEGMENT));
    double** a = malloc(ISIZE * sizeof(double*));
    for (int i = 0; i < ISIZE; i++)
        a[i] = malloc(JSIZE * sizeof(double));
    double** buf = malloc(block_size * sizeof(double*));
    for (int i = 0; i < block_size; i++)
        buf[i] = malloc(JSIZE * sizeof(double));

    //CREATING BORDERS FOR EACH PROCESS
    for (int i = 0; i < np; ++i) {
        data[i].start = i * block_size;
        data[i].end = (i + 1) * block_size;
    }
    data[np - 1].start = data[np-2].end;
    data[np - 1].end = ISIZE;

    //SENDING BORDERS FOR COMPUTATIONS
    for (int i = 1; i < np; i++) {
        MPI_Send(&data[i-1], 2, MPI_INT, i, i, MPI_COMM_WORLD);
    }
//    printf("PROCESS %d RECEIVED %d ... %d\n", rank, data[np - 1].start, data[np - 1].end);

    //COMPUTING PERSONAL RESULT
    for (int i = data[np - 1].start; i < data[np - 1].end; i++) {
        for (int j = 0; j < JSIZE; j++)
            a[i][j] = sin(0.00001*(10*i + j));
    }

    //RECEIVING RESULTS
    for (int i = 1; i < np; i++) {
        MPI_Recv(&(buf[0][0]), block_size*JSIZE, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int k = 0; k < block_size; k++){
            for(int j = 0; j < JSIZE; j++)
                a[(i-1)*block_size+k][j]=buf[k][j];
        }
    }
    double end_time = MPI_Wtime();

    //PRINT RESULTS IN FILE
    FILE* ff = fopen("results.txt","w");
    for(int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++)
            fprintf(ff,"%lf ",a[i][j]);
        fprintf(ff,"\n");
    }
    fclose(ff);

    //FREE MEMORY
    for (int i = 0; i < ISIZE; i++)
        free(a[i]);
    free(a);
    for (int i = 0; i < block_size; i++)
        free(buf[i]);
    free(buf);
    free(data);

    return end_time;
}

int slave(int np){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //ALLOCATE MEMORY
    int block_size = ISIZE / np;
    SEGMENT segment;
    double** a = malloc(block_size * sizeof(double*));
    for (int i = 0; i < block_size; i++)
        a[i] = malloc(JSIZE * sizeof(double));

    //RECEIVING BORDERS FOR COMPUTATIONS
    MPI_Recv(&segment, 2, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printf("PROCESS %d RECEIVED %d ... %d\n", rank, segment.start, segment.end);

    //COMPUTING
    for (int i = segment.start; i < segment.end; i++){
        for (int j = 0; j < JSIZE; j++)
            a[i-segment.start][j] = sin(0.00001*(10*i + j));
    }
    //SENDING RESULTS
    MPI_Send(&(a[0][0]), block_size*JSIZE, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);

    //FREE MEMORY
    for (int i = 0; i < block_size; i++)
        free(a[i]);
    free(a);

    return 0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double start_time = MPI_Wtime();
    double end_time;

    int np;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        end_time = master(np);
    }
    else {
        slave(np);
    }

    if (rank == 0) {
        printf("TOTAL TIME OF EXECUTION is %lf sec\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#define ISIZE 1000
#define JSIZE 1000

int main(int argc, char *argv[])
{
    int n_pr = atoi(argv[1]);
    printf("%d\n", n_pr);
    MPI_Init(&argc, &argv);


    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    for (i=0; i<ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.00001*a[i][j]);
        }
    }
    ff = fopen("result.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}

int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    for (i=0; i<ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.00001*a[i][j]);
        }
    }
    ff = fopen("result.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}
*/