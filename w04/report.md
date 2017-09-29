1. g++ -I/curc/sw/intel/16.0.3/impi/5.1.3.210/intel64/include -L/curc/sw/intel/16.0.3/impi/5.1.3.210/intel64/lib/release_mt -L/curc/sw/intel/16.0.3/impi/5.1.3.210/intel64/lib -Xlinker --enable-new-dtags -Xlinker -rpath -Xlinker /curc/sw/intel/16.0.3/impi/5.1.3.210/intel64/lib/release_mt -Xlinker -rpath -Xlinker /curc/sw/intel/16.0.3/impi/5.1.3.210/intel64/lib -Xlinker -rpath -Xlinker /opt/intel/mpi-rt/5.1/intel64/lib/release_mt -Xlinker -rpath -Xlinker /opt/intel/mpi-rt/5.1/intel64/lib -lmpicxx -lmpifort -lmpi -lmpigi -ldl -lrt -lpthread


2. Amdahl's law paper summary
   Amdahl's law states that only certain part of code can be parallelised and increasing the parallel resources after a certain limit for a fixed value of n does not improve the system's performance.
   By combining multiple BCE(baseline core) into a single powerful core can increase both the sequential and parallel speedup when perf(r) > r.
   When the portion of the code that needs to be parallelised is very low, parallel systems play a very minimal role in increasing the speedup.
   The equations described in the paper tells us that combining r number of  BCE can give us a sequential speedup of root(r).
   This paper also describes the speedup achieved using Amdahl's law by combining BCEs in various combinations.
   It also describes us that arranging the BCEs assymetrically can give us a more improved speedup on the sequential code than arranging symetrically. (Asymmetrically meaning - each core having different performance capabilities).
   
   At last since symetrical combination of BCEs give boost to parallel parts and asymetrical combination gives boost to sequential parts, dynamically changing the design combination of BCEs can help us to have both parallel and sequential   speedup.
    
