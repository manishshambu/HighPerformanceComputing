1. Running quicksort
quicksortjob.sh will run all combinations of n and p
It will genearate a file with randomly generated long ints.
Then it calls quicksort
To run the job - ./quicksortjob.sh

2. Running histogram
histogramjob.sh will run all combinations of n, p and number of bins
It will generate a file with randomly genrated double varaibles between 0 and 1
Then it calls histogram
To run the job - ./histogramjob.sh


2) Reason about he performance expectation of your code (apply the metric we discussed in class).
This demonstrates the strong scaling feature. We are keeping n constant and when we increase the number of procs, then the time reduces. 