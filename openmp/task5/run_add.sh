#!/bin/bash
gcc -fopenmp task5_add.c -o task5_add
OMP_NUM_THREADS=4 ./task5_add 20
