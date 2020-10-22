#!/bin/bash
gcc -fopenmp bugged4.c -o bugged4
OMP_NUM_THREADS=8 ./bugged4
