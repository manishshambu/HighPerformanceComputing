/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 * @brief openmp utility functions
 */

#ifndef _OMP_UTILS_H_
#define _OMP_UTILS_H_

#ifdef _OPENMP
#include <omp.h>
#define NOW()(omp_get_wtime())
#else
#define NOW() 0
#define omp_get_thread_num() 0
#endif

#endif //_OMP_UTILS_H_
