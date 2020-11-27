#!/bin/bash
mpicc lab.c -lm -o lab
mpirun -np 20 ./lab
