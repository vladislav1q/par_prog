#!/bin/bash
gcc -fopenmp task6.c -o task6 -lm
OMP_NUM_THREADS=4 ./task6 1000
