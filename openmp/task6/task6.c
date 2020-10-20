#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

//algorithm was taken from:
//https://intuit.ru/studies/courses/5938/1074/lecture/16450
//Параллельный алгоритм №1: декомпозиция по данным
//
//Theoretical validity:
//http://science-konspect.org/?content=10146
//Теорема о наименьшем простом делителе составного числа

struct number {
    int value;
    int prime_flag;
} number={0,1};

struct prime_numbers {
    int *values;
    int size;
} prime_numbers;

// computes prime numbers according to sieve of Eratosthenes
void get_primes(struct number * numbers, struct prime_numbers *primeNumbers, int count);

int main(int argc, char **argv){
    int prime_numbers_count = 0;
    int N = atoi(argv[1]);
    int N_sequent = sqrt(N) + 1;

    //initialization
    struct prime_numbers primeNumbers;
    primeNumbers.values = malloc(N_sequent*sizeof(int));
    primeNumbers.size = 0;
    struct number * numbers = malloc(N * sizeof(number));
    for (int i = 0; i <= N; i++) {
        numbers[i].value = i+2;
        numbers[i].prime_flag = 1;
    }

    double start_time = omp_get_wtime();
    //find prime numbers from 2 to N^0.5
    //by sequent sieve of Eratosthenes algorithm

    get_primes(numbers, &primeNumbers, N_sequent);
    //find prime numbers from N^0.5 to N
    //by parallel search
    #pragma omp parallel shared(N, N_sequent, primeNumbers, numbers)
    {
        #pragma omp for schedule(static)
        for (int i = N_sequent; i < N; i++) {
            for (int j = 0; j < primeNumbers.size; j++) {
                if (numbers[i].value % primeNumbers.values[j] == 0)
                    numbers[i].prime_flag = 0;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        if(numbers[i].prime_flag){
            prime_numbers_count++;
//            printf("%d ", numbers[i].value);
        }
    }
    printf("\n");

    printf("execution time: %f\n", omp_get_wtime()-start_time);
    printf("Number of prime numbers from 2 to %d is %d\n", N, prime_numbers_count);

    free(numbers);
    free(primeNumbers.values);

    return 0;
}

void get_primes(struct number * numbers, struct prime_numbers *primeNumbers, int count){
    //point all not prime numbers
    for (int i = 0; i < count; i++) {
        if(numbers[i].prime_flag){
            primeNumbers->size++;
            for (int j = i + numbers[i].value; j < count; j += numbers[i].value) {
                numbers[j].prime_flag = 0;
            }
        }
    }
    //write prime number in structure
    int j = 0;
    for (int i = 0; i < count; i++) {
        if(numbers[i].prime_flag){
            primeNumbers->values[j] = numbers[i].value;
            j++;
        }
    }

//    printf("Hello!\n");
//    for (int i = 0; i < primeNumbers->size; i++) {
//        printf("%d ", numbers[i].value);
//    }
//    printf("\nHello!\n");

    return;
}
