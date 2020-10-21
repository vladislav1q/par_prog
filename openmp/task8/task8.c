#include <omp.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//This is a realisation of quick sort
//works too long due to the restrictions on memory usage

//moves one element of array to the end
void move_element(int* arr, int length, int index);
//moves one element from the end of array to certain position
void insert_element(int* arr, int length, int index);
//sequent quicksort
void quicksort(int* arr, int length);
//sequent resulting mergesort
void mergesort(int* arr, int length, int n_threads);
//comparator for standard qsort
int comp (const int *i, const int *j);

int main(int argc, char** argv) {
    srand(time(NULL));

    int length = 10;
    int *arr = malloc(length*sizeof(int));

    //random array
    for(int i = 0; i < length; i++){
        arr[i] = rand()%10+1;
    }

    //array from console
    for(int i = 0; i < length; i++){
        scanf("%d", &arr[i]);
    }

    //array from file
//    FILE *file;
//    file = fopen("array.txt","r");
//    for (int i = 0; i < length; i++){
//        fscanf(file, "%d ", &arr[i]);
//    }

    //print array
    for(int i = 0; i < length; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    #pragma omp parallel shared(arr, length)
    {
        double start_time = omp_get_wtime();
        int id = omp_get_thread_num();
        int n_threads = omp_get_max_threads();
        int part_size = length/(float)n_threads;

        //slave processes sort parts of arr of size 'part_size'
        //master process computes residual part
        if(id != 0){
            quicksort(arr + part_size*(id-1),part_size);
        } else {
            quicksort(arr + part_size*(n_threads-1),length-part_size*(n_threads-1));
        }

        printf("Time of execution on %d proc = %lf\n",id, omp_get_wtime()-start_time);

        #pragma omp barrier

        if(id == 0){
            mergesort(arr, length, n_threads);
            printf("Time of total execution = %lf\n",omp_get_wtime()-start_time);
        }

    }

    //print array
//    for(int i = 0; i < length; i++){
//        printf("%d ", arr[i]);
//    }
//    printf("\n");


    //check standard qsort time execution
    for(int i = 0; i < length; i++){
        arr[i] = rand()%10+1;
    }

    time_t t0 = time(0);
    qsort(arr, length, sizeof (int), (int(*) (const void *, const void *)) comp);
    time_t t1 = time(0);

    double time_in_seconds = difftime(t1, t0);
    printf("Time of total execution for standart qsort = %lf\n",time_in_seconds);

    free(arr);

    return 0;
}

void mergesort(int* arr, int length, int n_threads){
    int part_size = length/(float)n_threads;
    int buffer;
    for(int i = part_size; i < length; i++){
        buffer = arr[i];
        for(int j = 0; j < i; j++){
            if(buffer <= arr[j]){
                insert_element(arr,i,j);
                break;
            }
        }
    }
}

void insert_element(int* arr, int length, int index){
    int buffer = arr[length];

    for(int i = length; i > index; i--){
        arr[i]=arr[i-1];
    }
    arr[index] = buffer;

    return;
}

void quicksort(int* arr, int length){
    //end if array consist of less than one element
    if(length <= 1)
        return;

    //last element in array is chosen as support
    //median is better, but computationally complex
    int support_el = arr[length-1];
    //move elements, bigger than support element
    //to the right from support element
    int k = 0;
    for(int i = 0; i < length-1;i++){
        if(arr[k]>=support_el){
            move_element(arr, length, k);
        }else{
            k++;
        }
    }
    //recursive launch for 2 parts of initial array
    //bigger than support and smaller than support
    quicksort(arr,k);
    quicksort(arr+k+1,length-k-1);

    return;
}

void move_element(int* arr, int length, int index){
    int buffer = arr[index];

    for(int i = index; i < length-1; i++){
        arr[i]=arr[i+1];
    }
    arr[length-1] = buffer;

    return;
}

int comp (const int *i, const int *j)
{
    return *i - *j;
}
