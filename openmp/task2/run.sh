#!/bin/bash
gcc -fopenmp task2.c -o task2
OMP_NUM_THREADS=8 ./task2
