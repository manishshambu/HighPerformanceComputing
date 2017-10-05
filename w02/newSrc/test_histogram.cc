/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 * @brief Tester for histogram
 */

#include "histogram.h"
#include <cstdlib> //rand
#include <cassert>
#include <cmath> //power
#include <iostream>
#include "omp-utils.h"
#include <cstdio>

void fill_vec(size_t n, double *x){

    if (x == NULL) throw "Vector x cannot be NULL";
    for (size_t ii(0);ii<n;++ii)
        x[ii] = 1e-6*(rand() % 1000000); /* between 0 and 1 */
}

int main(int argc, char** argv){

    // repeatable test
    srand(2017);

    // commandline arguments
    if (argc<3){
        printf("Log10 of array size and number of bins are mandatory arugments for %s.\n", __FILE__);
        exit(1);
    }

//    size_t n(std::pow(10,atoi(argv[1])));
//    size_t nb(std::pow(10,atoi(argv[2])));
      size_t n(atoi(argv[1]));
      size_t nb(atoi(argv[2]));

    double *x(new double[n]);
    fill_vec(n,x);
    if (argv[3])
	{
	 x[n-1]=nb;
	printf("argv[3], %d", atoi(argv[3]));
	}

    double *bb(NULL);
    size_t *bc(NULL);

    int nt = omp_get_max_threads();
    printf("Histogram of a random array of size %d, num_bins=%d, num_threads=%d\n",n,nb,nt);
    double tic(NOW());
    histogram(n, x, nb /*num_bins*/, &bb /*bin_brry*/, &bc /*bin_count*/);
    double toc(NOW());
    printf("Elapsed time=%-5.2e\n",toc-tic);

    delete[] x;
    delete[] bb;
    delete[] bc;
}
