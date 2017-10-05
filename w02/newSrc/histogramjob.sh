#!/bin/bash

#SBATCH --time=0:05:00 #walltime, abbrevated by -t
#SBATCH --nodes=1      # number of cluster nodes, abbrevated by -N
#SBATCH -o histogramBinOutlierF-%j.out # name of the stdout redirection file, using the job
#SBATCH -e histogramBinOutlierF-%j.err # name of the stderr redirection file
#SBATCH --ntasks 1     # number of parallel process
#SBATCH --qos debug # quality of service/queue
#SBATCH --cpus-per-task 24 #Controls the number of cpus allocated for a task

#run the program
echo "Running histogram on $(hostname --fqdn)"


for num_bins in 10 1000 10000;
do
	for num_procs in 1 2 4 8 12 16 20 24;
	do
		export OMP_NUM_THREADS=$num_procs;
		./test_histogram.exe 100 $num_bins;
	done
done
