#!/bin/bash
gcc -fopenmp bugged2.c -o bugged2
OMP_NUM_THREADS=8 ./bugged2
