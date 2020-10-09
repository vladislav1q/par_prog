#!/bin/bash
gcc -fopenmp task4.c -o task4
OMP_NUM_THREADS=10 ./task4
