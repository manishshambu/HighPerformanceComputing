#include <iostream>
#include <cstdlib>
#include <stdio.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)
#define CHK(expr) { cl_int _ret=expr; if(_ret){ std::cerr<<"Function call in "<<__FILE__<<", line "<<__LINE__<<" failed with error="<<_ret<<std::endl; abort();}}

void prepare_cmd_queue(cl_device_id &device_id, cl_context &context, cl_command_queue &queue)
{
    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    CHK( clGetPlatformIDs(1, &platform_id, &ret_num_platforms));
    CHK( clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_devices))
    
    // Create an OpenCL context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    CHK( ret );
    
    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &ret);
    CHK( ret );
}

void load_program(const char *fname, const cl_device_id device_id, const cl_context &context, cl_program &program, cl_kernel &kernel)
{
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
    cl_int ret;
    
    fp = fopen(fname, "r");
    if (!fp)
    {
        std::cerr<<"Failed to load kernel from "<<fname<<std::endl;
        abort();
    }
    
    source_str = (char*) malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    
    // Create a program from the kernel source
    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &ret);
    CHK(ret);
    
    // Build the program
    CHK( clBuildProgram(program, 1, &device_id, NULL, NULL, NULL));
    
    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "mandelbrot", &ret);
    CHK( ret );
    
}

int main(void)
{
    // Prepare OpenCL
    cl_device_id device_id = NULL;
    cl_context context;
    cl_command_queue queue;
    prepare_cmd_queue(device_id, context, queue);
    
    cl_program program;
    cl_kernel mandelbrot;
    
    load_program("mandelbrot.cl", device_id, context, program, mandelbrot);
    
    const size_t n = 256;
    double xmin = 1.0;
    double xmax = 2.0;
    double ymin = -2.0;
    double ymax = -1.0;
    int max_iter = 200;
    printf("Hello from main\n");
    
    
    int *mandelbrotArray = (int*) malloc(sizeof(int)*n*n);
    
    // Create Buffers
    cl_int ret;
    size_t byte_sz = n*n*sizeof(int);
    size_t double_sz = sizeof(double);
    size_t int_sz = sizeof(int);
    cl_mem mandelbrot_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE , byte_sz, NULL, &ret); CHK( ret );
    cl_mem xmin_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , double_sz, NULL, &ret); CHK( ret );
    cl_mem xmax_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , double_sz, NULL, &ret); CHK( ret );
    cl_mem ymin_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , double_sz, NULL, &ret); CHK( ret );
    cl_mem ymax_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , double_sz, NULL, &ret); CHK( ret );
    cl_mem n_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , int_sz, NULL, &ret); CHK( ret );
    cl_mem maxiter_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY , int_sz, NULL, &ret); CHK( ret );
    
    // Set the arguments of the kernel mandelbrot
    CHK( clSetKernelArg(mandelbrot, 0 /* arg index */, sizeof(xmin_buffer), &xmin_buffer) );
    CHK( clSetKernelArg(mandelbrot, 1 /* arg index */, sizeof(xmax_buffer), &xmax_buffer) );
    CHK( clSetKernelArg(mandelbrot, 2 /* arg index */, sizeof(ymin_buffer), &ymin_buffer) );
    CHK( clSetKernelArg(mandelbrot, 3 /* arg index */, sizeof(ymax_buffer), &ymax_buffer) );
    CHK( clSetKernelArg(mandelbrot, 4 /* arg index */, sizeof(n_buffer), &n_buffer) );
    CHK( clSetKernelArg(mandelbrot, 5 /* arg index */, sizeof(maxiter_buffer), &maxiter_buffer) );
    CHK( clSetKernelArg(mandelbrot, 6 /* arg index */, sizeof(mandelbrot_buffer), &mandelbrot_buffer) );
    
    // Copy the lists to the buffer
    CHK( clEnqueueWriteBuffer(queue, xmin_buffer, CL_TRUE, 0, double_sz, &xmin, 0, NULL, NULL) );
    CHK( clEnqueueWriteBuffer(queue, xmax_buffer, CL_TRUE, 0, double_sz, &xmax, 0, NULL, NULL) );
    CHK( clEnqueueWriteBuffer(queue, ymin_buffer, CL_TRUE, 0, double_sz, &ymin, 0, NULL, NULL) );
    CHK( clEnqueueWriteBuffer(queue, ymax_buffer, CL_TRUE, 0, double_sz, &ymax, 0, NULL, NULL) );
    CHK( clEnqueueWriteBuffer(queue, n_buffer, CL_TRUE, 0, int_sz, &n, 0, NULL, NULL) );
    CHK( clEnqueueWriteBuffer(queue, maxiter_buffer, CL_TRUE, 0, int_sz, &max_iter, 0, NULL, NULL) );
    
    // Execute the OpenCL kernel
    size_t global_item_size = n*n; // Process the entire lists
    size_t local_item_size  = 1;   // Process in groups of 8
    CHK( clEnqueueNDRangeKernel(queue, mandelbrot, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL) );
    
    // Read the memory buffer C on the device to the local variable C
    CHK( clEnqueueReadBuffer(queue, mandelbrot_buffer, CL_TRUE, 0, byte_sz, mandelbrotArray, 0, NULL, NULL) );
    
    // Display the result to the screen
    //for(size_t ii = 0; ii < n*n; ++ii)
    //std::cout<<mandelbrotArray[ii]<<std::endl;
    
    // Shutting down and clean up
    CHK( clFlush(queue) );
    CHK( clFinish(queue) );
    CHK( clReleaseKernel(mandelbrot) );
    CHK( clReleaseProgram(program) );
    CHK( clReleaseMemObject(xmin_buffer) );
    CHK( clReleaseMemObject(xmax_buffer) );
    CHK( clReleaseMemObject(ymin_buffer) );
    CHK( clReleaseMemObject(ymax_buffer) );
    CHK( clReleaseMemObject(n_buffer) );
    CHK( clReleaseMemObject(maxiter_buffer) );
    CHK( clReleaseMemObject(mandelbrot_buffer) );
    
    CHK( clReleaseCommandQueue(queue) );
    CHK( clReleaseContext(context) );
    
    
    free(mandelbrotArray);
    
    return 0;
}

