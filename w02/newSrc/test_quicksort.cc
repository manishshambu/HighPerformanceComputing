/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 */

#include "quicksort.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "omp-utils.h"
#include <algorithm>
#include <cstdio>

void fill(size_t n, long *x){
    if (x == NULL) throw "Vector x cannot be NULL";
    for (size_t ii(0);ii<n;++ii)
        x[ii] = rand();
}

void main(size_t argc, char** argv){

    // repeatable test
    srand(2017);

    // commandline arguments
    if (argc<2){
        printf("Array size is a mandatory arugments for %s.\n", __FILE__);
        exit(1);
    }

    size_t n(atoi(argv[1]));
    printf("Sorting a random array of size %dK, num_threads=%d\n",n/1000,omp_get_max_threads());
    long *x = new long[n];
    long *y = new long[n];
    fill(n,x);

    double tic=NOW();
    quicksort(x,n,y);
    double toc=NOW();
    printf("Elapsed=%5.2e\n",toc-tic);

    //checking
    std::sort(x,x+n);
    for (size_t ii=0;ii<n;++ii)
        assert (x[ii]==y[ii]);

    delete[] x;
    delete[] y;
}
