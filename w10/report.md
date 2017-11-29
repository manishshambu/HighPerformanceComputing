1. Parallel work is submitted to devices by launching kernels. To deploy the kernel to a device one uses  clEnqueueNDRangeKernel. Not only does this function deploy kernels to devices, it also identifies how many work-items should be generated to execute the kernel (global_size) and the number of work-items in each work-group (local_size).

2. On Mac it supports only local_size = 1 for CPU devices. (There is a limitation on this )
   However when I could change the local_size variable for GPU devices.
   When I increased the local_size value, then my program execution became faster relatively. This is because, when the group size is large, then fewer kernels will be called by the host program. Increasing locacl_size value increases
   the speed only till a certain limit, later it starts saturating and decreases, since amount of parallelism reduces.

3. n = 256, 0.034s
   n = 512, 0.035s
   n = 1024,0.042s
   n = 2048,0.049s
