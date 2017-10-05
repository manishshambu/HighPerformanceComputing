1.
 Quicksort implementation difference.

In my code I used only 1 type of parallelism in my code i.e I used only omp tasks for the recursive calls of quicksort.
However after analysis, I came to know that there is another type of parallelism that can be incorporated within the partition function of quicksort.
When I tried to use omp for within the parallel tasks sections, my program hung indefinitely, and thats why I could not implement parallelism inside the partition function.

 Histogram implementation difference.
In my histogram program I have used lot many openmp barriers for synchronisation. Also a critical section to update the global count array.
The given program however uses very less thread synchronisations. Other than that, the approach I took is pretty much the same as the given program

2.

3.
These are the threshold values which tell the processor  when to execute task parallelism or work sharing, so that the performance remains in its peak.


4. 
This environmanet variable helps us to determine the machine topology and assign OpenMP threads to the processors based upon their physical location in the machine.
Specifying scatter distributes the threads as evenly as possible across the entire system.
