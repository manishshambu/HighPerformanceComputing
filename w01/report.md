1. What does the man page for bash says for PATH? Use man bash to print the man page.
   The search path for commands.  It is a colon-separated  list  of
   directories  in  which the shell looks for commands (see COMMAND
   EXECUTION below).  A zero-length (null) directory  name  in  the
   value of PATH indicates the current directory.  A null directory
   name may appear as two adjacent colons,  or  as  an  initial  or
   trailing  colon.   The  default path is system-dependent, and is
   set by the administrator who installs bash.  A common  value  is
   ‘‘/usr/gnu/bin:/usr/local/bin:/usr/ucb:/bin:/usr/bin’’.


2. What is the entry for LD_LIBRARAY_PATH in the man page for ld.so (dynamic linker on Linux)?
   A colon-separated list of directories in which to search for ELF
   libraries at execution-time.  Similar to  the  PATH  environment
   variable.


3. What is the value stored in HOME. You can use echo to show the value of a variable.
   /home/mash3503


4. Inspect the details of intel module on Summit. How does it change the PATH and LD_LIBRARY_PATH variables? 
   PATH variable in the intel module - /curc/tools/x86_64/rh6/software/intel/15.0.2/composer_xe_2015.2.164/bin/intel64:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin
   PATH variable before loading intel module - usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin

   PATH variable chanages for each module.
 
   LD_LIBRARY_PATH variable in the intel module - /curc/tools/x86_64/rh6/software/intel/15.0.2/composer_xe_2015.2.164/compiler/lib/intel64
   LD_LIBRARY_PATH variable before loading intel module - ''

5. After you load the intel module, how does the list of available modules change?
   After loading the intel module, few set of MPI implementation and compiler dependant application adds to the total list of available software.

6. Descibe the motivation for hierarchical module system.
   In a hierarchical module system, modules are only available to be loaded once their dependencies have been satisfied. This prevents accidental loading of modules that are inconsistent with each other.

7. What environment variables are set by the intel and gcc modules?
   The following environmental variables are set when intel module is loaded.
   "CURC_INTEL_ROOT","/curc/tools/x86_64/rh6/software/intel/15.0.2"
   "CURC_INTEL_BIN","/curc/tools/x86_64/rh6/software/intel/15.0.2/bin"

   The following environmental variables set when gcc module is loaded.  
   "CURC_GCC_ROOT","/curc/tools/x86_64/rh6/software/gcc/5.1.0"
   "CURC_GCC_LIB","/curc/tools/x86_64/rh6/software/gcc/5.1.0/lib"
   "CURC_GCC_INC","/curc/tools/x86_64/rh6/software/gcc/5.1.0/include"
   "CURC_GCC_BIN","/curc/tools/x86_64/rh6/software/gcc/5.1.0/bin"

9. Install a compatible version of Intel and/or GCC compiler on a machine that you can freely access. There are times that RC is not available due to maintenance, project and assignment deadlines will not be extended due to that. What does ${CC} -v (where CC is either gcc or icc) print on your machine?
   "Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
   Apple LLVM version 8.1.0 (clang-802.0.42)
   Target: x86_64-apple-darwin16.7.0
   Thread model: posix
   InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin"

10. Store the lines above in job.sh file and use sbatch job.sh to submit to Summit. Monitor the status of the job by using commands given in the related RC guide section. What commands did you use? What are the contents of the .out and .err files. If the contents are different from what you saw interactively, debug.
   Contents of .err file is empty
   Contents of .out file
   "Running on shas0508.rc.int.colorado.edu
   Calling axpy - Passed" 
   
   I monitored the job using scontrol show job {JOBID}.

11. Does the behavior of compiler or program change? Address the compiler warning you received by adding proper flag (consult the compiler man page man icc). How does the program behavior change after compilation?    
    Compiler warning
    test_axpy.cc:(.text+0x61): undefined reference to `omp_get_wtime'
    test_axpy.cc:(.text+0xe9): undefined reference to `omp_get_wtime' 
   
     To address this we need to link I added -l flag which links the omp library. The command I used to compile is "g++  -D_OPENMP axpy.cc test_axpy.cc -o test_axpy.exe -lgomp"    

12. Set OMP_NUM_THREADS to 1, 2, 4, 8, and 16 and report the results.
   [mash3503@login01 src]$ OMP_NUM_THREADS=2 ./test_axpy.exe 
   Calling axpy - Passed
   Elapsed time: 6.8187s
   [mash3503@login01 src]$ OMP_NUM_THREADS=4 ./test_axpy.exe 
   Calling axpy - Passed
   Elapsed time: 5.69018s
   [mash3503@login01 src]$ OMP_NUM_THREADS=8 ./test_axpy.exe 
   Calling axpy - Passed
   Elapsed time: 5.88143s
   [mash3503@login01 src]$ OMP_NUM_THREADS=12 ./test_axpy.exe 
   Calling axpy - Passed
   Elapsed time: 5.45118s
