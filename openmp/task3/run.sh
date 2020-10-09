#!/bin/bash
gcc -fopenmp task3.c -o task3
OMP_NUM_THREADS=8 ./task3
