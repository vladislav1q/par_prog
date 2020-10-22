#!/bin/bash
gcc -fopenmp bugged3.c -o bugged3
OMP_NUM_THREADS=8 ./bugged3
