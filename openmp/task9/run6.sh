#!/bin/bash
gcc -fopenmp bugged6.c -o bugged6
OMP_NUM_THREADS=8 ./bugged6
