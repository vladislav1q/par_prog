#!/bin/bash
gcc -fopenmp task2_add1.c -o task2_add1
OMP_NUM_THREADS=6 ./task2_add1 1000
