#!/bin/bash
gcc -fopenmp bugged5.c -o bugged5
OMP_NUM_THREADS=5 ./bugged5
