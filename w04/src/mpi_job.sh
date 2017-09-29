#!/bin/bash

#SBATCH --time=0:05:00
#SBATCH -o axpy-%.out
#SBATCH -e axpy-%j.err
#SBATCH --qos debug

# advice task manager that maximum of 4
# tasks/processes may be spawned
#SBATCH --ntasks 4

# run the program
mpirun -n 4 ./mpi_hello_world
