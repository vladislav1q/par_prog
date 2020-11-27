#!/bin/bash
mpicc lab2.c -lm -o lab2
mpirun -np 4 ./lab2
