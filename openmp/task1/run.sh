#!/bin/bash
gcc -fopenmp task1.c -o task1
OMP_NUM_THREADS=8 ./task1
