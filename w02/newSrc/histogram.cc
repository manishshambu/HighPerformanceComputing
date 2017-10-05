/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 */

#include "histogram.h"
#include <cstdio>
#include <cassert>
#include "omp-utils.h"

void histogram(size_t n, const double *x, int num_bins,
    double **bin_bdry, size_t **bin_count){

    // master tasks
    int nt = omp_get_max_threads();
    double *bb = new double[num_bins+1]; *bin_bdry = bb;
    size_t *bc = new size_t[num_bins];   *bin_count = bc;
    size_t *local_bc = new size_t[nt*num_bins];
    //in local_bc each row is for a thread to avoid false sharing

    // compute min/max
    double mn(x[0]),mx(x[0]);
    #pragma omp parallel shared(mn,mx)
    {
        int r = omp_get_thread_num();

        #pragma omp for reduction(min:mn)
        for (size_t ii=0; ii < n; ++ii)
            if (x[ii]<mn) mn = x[ii];

        #pragma omp for reduction(max:mx)
        for (size_t ii=0; ii < n; ++ii)
            if (x[ii]>mx) mx = x[ii];

        // implicit barrier after for
        #pragma omp single
        {
          mn -= 1e-14;
          mx += 1e-14;
          bb[num_bins]=mx;
        }

        // to make sure it is consistent among threads
        #pragma omp flush (mn,mx)

        // Setting the counters to zero
        double w=(mx-mn)/num_bins;
        #pragma omp for
        for (int jj=0; jj<nt; ++jj)
            for (size_t ii=0; ii<num_bins; ++ii)
                local_bc[jj*num_bins+ii]=0;

        // Setting bin boundary (only needed for error checking)
        #pragma omp for
        for (size_t ii=0; ii<num_bins; ++ii){
            bb[ii]=mn+ii*w;
            bc[ii]=0;
        }

        #pragma omp for
        for (size_t ii(0);ii<n; ++ii){
            int idx = int((x[ii]-mn)/w);
            // error checking, can be turned off
            assert (idx < num_bins && idx>=0);
            assert (x[ii]>=bb[idx] && x[ii]<bb[idx+1]);
            ++local_bc[r*num_bins+idx];
        }

        //don't expect to gain speedup using parallel reduction
        #pragma omp for
        for (size_t ii(0);ii<num_bins; ++ii)
            for (int jj(0);jj<nt;++jj)
                bc[ii] += local_bc[jj*num_bins+ii];
    }
    return;
}
