1. The search path for commands.  It is a colon-separated  list  of
   directories  in  which the shell looks for commands (see COMMAND
   EXECUTION below).  A zero-length (null) directory  name  in  the
   value of PATH indicates the current directory.  A null directory
   name may appear as two adjacent colons,  or  as  an  initial  or
   trailing  colon.   The  default path is system-dependent, and is
   set by the administrator who installs bash.  A common  value  is
   ‘‘/usr/gnu/bin:/usr/local/bin:/usr/ucb:/bin:/usr/bin’’.


2. A colon-separated list of directories in which to search for ELF
   libraries at execution-time.  Similar to  the  PATH  environment
   variable.


3. /home/mash3503


5. PATH variable in the intel module - /curc/tools/x86_64/rh6/software/intel/15.0.2/composer_xe_2015.2.164/bin/intel64:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin
  PATH variable before loading intel module - usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin
PATH variable chanages for each module.
 
LD_LIBRARY_PATH variable in the intel module - /curc/tools/x86_64/rh6/software/intel/15.0.2/composer_xe_2015.2.164/compiler/lib/intel64
LD_LIBRARY_PATH variable before loading intel module - ''

6. After loding the intel module, few set of MPI implementation adds to the total list of available software.

7. In a hierarchical module system, modules are only available to be loaded once their dependencies have been satisfied. This prevents accidental loading of modules that are inconsistent with each other.

8.
Environmental variables set when intel module is loaded.
AR, MANPATH, _ModuleTable003, HOSTNAME, INTEL_LICENCE_FILE,CURC_INTEL_BIN,TERM,SHELL, HISTSIZE, LMOD_DEFAULT, MODULEPATH_ROOT, SSH_CLIENT, CURC_INTEL_ROOT, LMOD_PKG, OLDPWD, LMOD_VERSION, MIC-LD-LIBRARY, SSH_TTY,USER, LD_LIBRARYPATH,LMOD_sys

Environmental variables set when gcc module is loaded.  
MANPATH, HOSTNAME, TERM, SHELL, HISTSIZE, LMOD_DEFAULT_MODULEPATH, MODULEPATH_ROOT, SSH_CLIENT, LMOD_PACKAGE_PATH, CURC_GCC_BIN, OLDPWD, SSH_TTY, USER, LD_LIBRARY_PATH, LMOD_sys, CC, CURC_FAMILY_COMPILER

9. Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
Apple LLVM version 8.1.0 (clang-802.0.42)
Target: x86_64-apple-darwin16.7.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

11.

12.
[mash3503@shas0136 src]$ OMP_NUM_THREADS=1 ./test_axpy.exe
Calling axpy - Passed
[mash3503@shas0136 src]$ OMP_NUM_THREADS=2 ./test_axpy.exe
Calling axpy - Passed
[mash3503@shas0136 src]$ OMP_NUM_THREADS=4 ./test_axpy.exe
Calling axpy - Passed
[mash3503@shas0136 src]$ OMP_NUM_THREADS=8 ./test_axpy.exe
Calling axpy - Passed
[mash3503@shas0136 src]$ OMP_NUM_THREADS=16 ./test_axpy.exe
Calling axpy - Passed  
