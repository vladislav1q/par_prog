#!/bin/bash
gcc -fopenmp task8.c -o task8
OMP_NUM_THREADS=8 ./task8
