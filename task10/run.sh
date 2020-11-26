#!/bin/bash
gcc -fopenmp task10.c -o task10
OMP_NUM_THREADS=4 ./task10 20
