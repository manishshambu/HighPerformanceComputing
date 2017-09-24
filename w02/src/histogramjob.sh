#!/bin/bash

#SBATCH --time=0:05:00 #walltime, abbrevated by -t
#SBATCH --nodes=1      # number of cluster nodes, abbrevated by -N
#SBATCH -o histogram-%.out # name of the stdout redirection file, using the job
#SBATCH -e histogram-%j.err # name of the stderr redirection file
#SBATCH ---ntasks 1     # number of parallel process
#SBATCH --qos debug # quality of service/queue

#run the program
echo "Running histogram on $(hostname --fqdn)"

declare -a arr=(2 4 8 16 32)
for n in 100 100000 1000000000;
do
	echo "Number of elements:"$n
	# Generate doubles randomly and store it in a file
	# ./generateElements numberOfElements 0 for histogram/1 for quicksort output to file names n.histo 
	./generateElements.exe $n 0 > $n.histo
        for num_procs in "${arr[@]}"
        do
                echo "Number of procs:"$num_procs
                omp_NUM_THREADS=$num_procs
		for num_bins in 100 100000 100000; 
		do
			echo "Number of bins:"$num_bins
			# ./histogram.exe numberOfBins noOfInputs fileName Without-Outlier=0/Outlier=1
                	time ./histogram.exe $num_bins $n $n.histo 0
		done
        done
done

