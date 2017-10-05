#!/bin/bash

#SBATCH --time=0:05:00 #walltime, abbrevated by -t
#SBATCH --nodes=1      # number of cluster nodes, abbrevated by -N
#SBATCH -o histogramStrong-%j.out # name of the stdout redirection file, using the job
#SBATCH -e histogramStrong-%j.err # name of the stderr redirection file
#SBATCH --ntasks 1      # number of parallel process
#SBATCH --qos debug # quality of service/queue
#SBATCH --cpus-per-task 24 #Controls the number of cpus allocated for a task

#run the program
echo "Running quicksort on $(hostname --fqdn)"

for i in {1,2,4,8,12,16,20,24};do export OMP_NUM_THREADS=$i; ./test_histogram.exe 1000 10;done
