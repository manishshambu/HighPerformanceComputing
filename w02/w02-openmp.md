## Introduction

In this lab session we explore the features of OpenMP API. Before moving forward, download and familiarize yourself with the [`omp-demo`](http://cims.nyu.edu/~rahimian/assets/dedicated/w01/omp-demo.tar) given in class. In class, we discussed OpenMP directives for work sharing and coordination.

The files related to this assignment are in `assignments/w02/src/` on bitbucket. Submission is also through bitbucket.

## Task parallelism

Task constructs are primarily useful for the recursive function implementation and irregular parallelism. Tasks have code to execute and a data environment. At first glance, they look very similar to the `sections/section` constructs.

Tasks are generated dynamically in recursive structures or while loops. When a thread encounters a task construct, it may choose to execute the task immediately or defer its execution until a later time. If task execution is deferred, then the task is placed in a *pool of tasks*. A thread that executes the task might be different from the one that encountered it. For tasks, there is a corresponding `taskwait` directive that specifies a wait on the completion of child tasks generated since the beginning of the current task.

As for data scope, the default for tasks is usually firstprivate, because the task may not be executed until later (and variables may have gone out of scope).

As an example, consider the computation of Fibonacci numbers using tasks

``` cpp
long fib(long n) {
    long i(0),j(0);
    if (n<2) return 1;

    #pragma omp task shared(i)
    i = fib(n-1);

    #pragma omp task shared(j)
    j = fib(n-2);

    #pragma omp taskwait
    return i+j;
}
```

Here, we create two tasks to compute *i* and *j* and the creating task is suspended at `taskwait`. To start the required number of threads, we call `fib` from within a parallel construct with a single directive

``` cpp
void demo_task(){

    long n(11),v;
    #pragma omp parallel shared (n,v)
    {
        #pragma omp single
        v = fib(n);
    }
}
```

To familiarize yourself with tasks, copy and compile the code above. Add print statements to different tasks and explore their execution order. In programming assignment below, we will use tasks to implement quicksort.

## Algorithm analysis assignments

Submit a single `w02/report-algo.(pdf|md|txt)` file through bitbucket, as the extension implies, your submission can be ascii text, markdown, or pdf.

<emph>Credits: 50 points.</emph><br><emph>Due: Wednesday, September 13, 3pm.</emph>

1.  Consider the problem of square matrix-matrix multiplication of two matrices with size *n* × *n*, where, for simplicity, *n* = 2<sup>*k*</sup> for some integer *k*. Consider the two PRAM cases below. Using DAGs discuss the optimal scheduling and the corresponding parallel work and depth.
    1.  There are *n*<sup>3</sup> processors denoted by *P*<sub>*i**j**k*</sub> with 0 ≤ *i*, *j*, *k* &lt; *n*.
    2.  When there are *p* = 2<sup>*q*</sup> &lt; *n*<sup>3</sup> processors.
2.  Consider the problem of computing *x*<sup>*n*</sup>, where *n* = 2<sup>*k*</sup> for some non-negative integer *k* and *x* a scalar. The repeated-squaring algorithm consists of computing *x*<sup>2</sup>, *x*<sup>4</sup> = *x*<sup>2</sup> ⋅ *x*<sup>2</sup> and so on.
    1.  Draw the DAG corresponding to this algorithm. What is the work and depth of this algorithm?
    2.  Repeat for the case when *x* is an *m* × *m* matrix.
3.  In the class, we discussed an the parallel sum (reduction) algorithm using the Work/Depth language model (for an array *x* of size *n*). State a PRAM algorithm with an arbitrary number of processors *p* ≤ *n*. Derive estimates for the work, time, speedup and efficiency as a function of the problem size *n* and the number of processors *p*.

4.  Suppose *T*<sub>*s**e**r**i**a**l*</sub> = *n* and *T*<sub>*p**a**r**a**l**l**e**l*</sub> = *n*/*p* + *l**o**g*(*p*),where times are in microseconds. If we increase *p* by a factor of *k*, find a formula for how much we will need to increase *n* in order to maintain constant efficiency. Is the parallel program scalable?

5.  Consider the loop

        a[0] = 0;
        for (i = 1; i < n; i++)
            a[i] = a[i−1] + i;

    There is clearly a loop-carried dependence, as the value of `a[i]` cannot be computed without the value of `a[i−1]`. Suggest a way to eliminate this dependence and parallelize the loop.

## Programming assignment

Submit your source code in `w02/src/` on bitbucket. Put your analysis in a text or pdf files `w02/report-sort.(pdf|md|txt)` and `w02/report-hist.(pdf|md|txt)`.

<emph>Credits: 100 points.</emph><br><emph>Due: Wednesday, September 20, 3pm.</emph>

1.  Our objective is to implement a parallel quicksort using OpenMP tasks. Your primary goal is to get good scalability. Tailor your recursion in a clever fashion. The signature of quicksort is declared in `src/quicksort.h`:

        /* @input x the array to be sorted
         * @input n size of array x
         * @output y the output array (already allocated)
         */
         void quicksort(const long *x, size_t n, long *y);

    1.  Outline your strategy for quicksort using OpenMP in pseudo-code or plain English.
    2.  Submit a quicksort code through bitbucket `w02/src/quicksort.cc`.
    3.  Perform strong and weak scaling analysis on a single node on Summit. For weak scaling use grain sizes of 1M and 10M long integer per thread. For strong scaling, work with a problem of size (number of physical cores) × {1M,10M}.

2.  We would like to implement a function that computes the histogram for a given set of data. The signature of the function you need to implement is given in `histogram.h`:
        /*
         * @input n the size of the data vector x
         * @input x data vector
         * @input num_bins the number of bins to use
         * @output bin_bdry array of size num_bins+1 holding the left and
         *         right limit of each bin
         * @output bin_count array of size num_bins holding the count of
         *         items in each bin.
         *
         * For both output arrays, memory is allocated by the histogram
         * function and the caller is responsible for deleting.
         */
         void histogram(size_t n, const double *x, int num_bins, /* inputs  */
                        double **bin_bdry, size_t **bin_count);  /* outputs */

    Boundaries of bins are chosen equi-spaced based on the input data such that `x[ii]>bin_bdry[0]` and `x[ii]<bin_bdry[num_bins]`. To simplify comparison, set the `bin_bdry[0]=mn-1E-14` and `bin_bdry[num_bins]=mx+1E-14` where `mn/mx` denote the minimum and maximum value of data.

    This function may be called in the following manner

        size_t  n = 10 //or 1e7
        double *x = new double[n]
        /* read x from file, etc. */
        int nb(3);
        double *bb;
        size_t *bc;
        histogram(n, x,  nb /*num_bins*/, &bb /*bin_bdry*/, &bc /*bin_count*/);

        for (int i(0);i<nb;++i)
            printf("[bin %d: %5.2e -- %5.2e] %d\n", i, bb[i],bb[i+1],bc[i]);

    and, for example, may produce a result:

        [bin 0: -#.00e-14 -- 3.00e+00] 3
        [bin 1: 3.00e+00 -- 6.00e+00] 4
        [bin 2: 6.00e+00 -- 9.00e+00] 3

    Use all the features of OpenMP to speed up your code.

    1.  Describe in pseudo-code or plain English, the parallel algorithm and OpenMP features you are using in to implement histogram.
    2.  Reason about he performance expectation of your code (apply the metric we discussed in class).
    3.  Submit the implementation for `histogram.cc` along with the plot for wall-clock time for the `histogram()` call with respect to number of thread for `n=1E2,1E5,1E9` and number of bins `num_bins=10,1E3,1E4`. For these tests, generate two data sets:
        1.  Uniformly distributed doubles between \[0, 1\].
        2.  Uniformly distributed doubles between \[0, 1\] and an outlier with value equal to `num_bins`.


