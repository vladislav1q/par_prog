#!/bin/bash
gcc -fopenmp task5.c -o task5
OMP_NUM_THREADS=4 ./task5 20
