#!/bin/bash

#SBATCH --time=0:05:00 #walltime, abbrevated by -t
#SBATCH --nodes=1      # number of cluster nodes, abbrevated by -N
#SBATCH -o quicksort-%j.out # name of the stdout redirection file, using the job
#SBATCH -e quicksort-%j.err # name of the stderr redirection file
#SBATCH --ntasks 1 	# number of parallel process
#SBATCH --qos debug # quality of service/queue

#run the program
echo "Running quicksort on $(hostname --fqdn)"

declare -a arr=(2 4 8 16 32)
for n in 1000000
do
	#echo "Number of elements:" $n
	./generateElements.exe $n 1 > $n.qs
	for num_procs in "${arr[@]}"
	do
		#echo "Number of procs: "$num_procs
		omp_NUM_THREADS=$num_procs
		time ./quicksort.exe $n $n.qs
	done
done
