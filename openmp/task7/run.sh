#!/bin/bash
gcc -fopenmp task7.c -o task7
OMP_NUM_THREADS=2 ./task7
