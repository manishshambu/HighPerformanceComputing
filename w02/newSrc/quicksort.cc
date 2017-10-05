/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 * @brief
 */

#include "quicksort.h"
#include <cmath>
#include <cassert>
#include "omp-utils.h"

/*
 * Threshold values to switch between work sharing (NWS) or task
 * parallelism. Should be tuned for each machine.
 */
#define NWS 5e4   /* minimum size for workshare  */
#define NTSK 5e4  /* size to switch to tasks     */
#define NMIN 50   /* minimum size for final task */

void scan_seq(int *x, size_t n){
    for(int ii=1; ii<n; ++ii) x[ii]+=x[ii-1];
}

void scan_par(int *x, size_t n){

    if (n<2) return;
    const int np=omp_get_max_threads();
    if (n<=np){
        for(int i(1);i<n;++i) x[i]+=x[i-1];
        return;
    }

    size_t *ps=new size_t[np];
    #pragma omp parallel default(none) firstprivate(x,n,ps)
    {
        size_t stride = floor(1.0*n/np);
        int r         = omp_get_thread_num();
        size_t offset = r*stride;
        if (r==np-1) stride=n-offset;

        for (size_t i=1;i<stride;++i)
            x[offset+i]+=x[offset+i-1];
        ps[r] = x[offset+stride-1];

        #pragma omp barrier
        #pragma omp single
        for (int i=1;i<np;++i)
            ps[i]+=ps[i-1];

        #pragma omp barrier
        if (r>0)
            for (size_t i=0;i<stride;++i)
                x[offset+i]+=ps[r-1];
    }
    delete[] ps;
}

void partition(const long *x, const size_t n, const long p, long *&L, size_t &lsz, long *&H, size_t &hsz){

    int *c = new int[n];
    int *l = new int[n];
    int *h = new int[n];

    #pragma omp parallel for default(none) firstprivate(c,l,h,x) if(n>NWS)
    for (size_t ii=0; ii<n; ++ii){
        l[ii] = c[ii] = x[ii]<=p;
        h[ii] = !l[ii];
    }

    // function pointer to scan
    typedef void (*scan_t)(int*,size_t);
    scan_t scan = (n>NWS) ? scan_par : scan_seq;

    scan(l,n);
    scan(h,n);

    lsz=l[n-1];
    hsz=h[n-1];
    assert (lsz+hsz==n);

    if (lsz>0) L = new long[lsz];
    if (hsz>0) H = new long[hsz];

    long* Lp(L), *Hp(H);
    #pragma omp parallel for default(none) firstprivate(l,h,c,x,Lp,Hp) if(n>NWS)
    for (size_t ii=0; ii<n; ++ii)
        if (c[ii]) Lp[l[ii]-1]=x[ii];
        else       Hp[h[ii]-1]=x[ii];

    delete[] c;
    delete[] l;
    delete[] h;

    return;
}

void quicksort_tsk(const long *x, size_t n, long *y){

    if (n<2) {
        for (size_t ii=0;ii<n;++ii) y[ii]=x[ii];
        return;
    }

    long p = x[n-1];
    long *L(NULL), *H(NULL);
    size_t lsz, hsz;
    partition(x,n-1,p,L,lsz,H,hsz);
    y[lsz] = p;

    #pragma omp task default(none) firstprivate(L,lsz,y) if(n>NMIN)
    {
        quicksort_tsk(L,lsz,y);
        delete[] L;
    }

    #pragma omp task default(none) firstprivate(H,hsz,lsz,y) if(n>NMIN)
    {
        quicksort_tsk(H,hsz,y+lsz+1);
        delete[] H;
    }

    return;
}

void quicksort(const long *x, size_t n, long *y){

    if (n<2) {
        for (size_t ii=0;ii<n;++ii) y[ii]=x[ii];
        return;
    }

    if (n<NTSK){
        #pragma omp parallel default(none) firstprivate(x,n,y)
        {
            #pragma omp single nowait
            quicksort_tsk(x,n,y);
        }
        return;
    }

    long p = x[n-1];
    long *L(NULL), *H(NULL);
    size_t lsz, hsz;
    partition(x,n-1,p,L,lsz,H,hsz);
    y[lsz] = p;

    quicksort(L,lsz,y);
    quicksort(H,hsz,y+lsz+1);

    delete[] L;
    delete[] H;
}
