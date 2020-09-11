#!/bin/bash
gcc -fopenmp task1_add1.c -o task1_add1
OMP_NUM_THREADS=6 ./task1_add1
