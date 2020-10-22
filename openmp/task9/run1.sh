#!/bin/bash
gcc -fopenmp bugged1.c -o bugged1
OMP_NUM_THREADS=8 ./bugged1
