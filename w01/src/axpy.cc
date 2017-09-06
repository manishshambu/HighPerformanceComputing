#include "axpy.h"
#include <omp.h>

void axpy(size_t n, double a, const double *x, double *y) throw (std::runtime_error)
{
	#pragma omp parallel
	{ 
		#pragma omp for
		for(int i = 0; i<n; i++)
		{
			y[i] = a * x[i] + y[i];
		}
	}
}
