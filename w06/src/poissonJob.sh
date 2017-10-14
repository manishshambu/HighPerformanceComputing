#!/bin/bash

#SBATCH --time=0:05:00
#SBATCH -o axpy-%j.out
#SBATCH -e axpy-%j.err
#SBATCH --qos debug

# advise task manager that maximum of 4
# tasks/processes may be spawned
#SBATCH --ntasks 4

# run the program

echo "Running poissons residual calculation on $(hostname --fqdn)"

for i in 1 4 9 16 25
do
        mpirun -n $i ./poisson.exe 200;
done

for i in 1 8 27
do
	mpirun -n $i ./poisson.exe 200;
done
