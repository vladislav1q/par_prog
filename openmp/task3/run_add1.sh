#!/bin/bash
gcc -fopenmp task3_add1.c -o task3_add1
OMP_NUM_THREADS=2 ./task3_add1
