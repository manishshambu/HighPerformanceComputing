#ifdef _OPENMP
#include <omp.h>
#define NOW()(omp_get_wtime())
#else
#define NOW() 0
#define omp_get_thread_num() 0
#endif

#include <iostream>
#include <unistd.h>
#include "histogram.h"
#include <stdio.h>
#include <stdlib.h>

double min_val = 9999999999.0;
double max_val = -1.0;
int nb = 0;
double bin_range = 0;
typedef double mytime_t;
int num_threads =0;

void findminmax(const double *x, size_t n)
{
	#pragma omp parallel for reduction(max:max_val) 
	for(int i=0; i < n; i++)
	{
		if(x[i] > max_val)
		{
			max_val = x[i];
		}
	}

	#pragma omp parallel for reduction(min: min_val)
	for(int i=0; i < n; i++)
	{
		if(x[i] < min_val)
		{
			min_val = x[i];
		}
	}
}
void histogram(size_t n, const double *x, int num_bins, double **bin_bdry, size_t **bin_count)
{
	findminmax(x, n);
	double* bb = new double[num_bins+1];
	size_t* bc = new size_t[num_bins];
	*bin_bdry = bb;
	*bin_count = bc;
	
	// Initialize bin boundaries and bin count to 0
	for(int i=0; i<num_bins; i++)
	{
		bb[i] = 0;
		bc[i] = 0;
	}
	bb[num_bins+1] = 0;

	double binRange = (max_val - min_val)/num_bins;
	mytime_t start = NOW();
	//omp_set_num_threads(32);
	#pragma omp parallel
	{
		size_t *histogram_private_count = new size_t[num_bins];
		num_threads = omp_get_num_threads();
		#pragma omp for
		for(size_t i =0; i<num_bins; i++)
		{
			histogram_private_count[i] = 0;
			bb[i] = min_val + binRange*i;
		}
	
		#pragma omp single
		bb[num_bins] = min_val + binRange*num_bins;
	
		#pragma omp barrier
		
		#pragma omp for
		for(int i=0; i<=num_bins; i++)
		{
			bb[i] = min_val + binRange*i;
		}
			
		#pragma omp barrier 
		#pragma single nowait
		{
			bb[0] -= 1E-14;
			bb[num_bins] += 1E-14;
		}
		
		#pragma omp barrier
		#pragma omp for
	       	for(size_t i=0; i<n; i++)
		{
			for(int j = 0; j <= num_bins; j++)
			{
				if(x[i] >= bb[j] && x[i] < bb[j+1])
				{	
					histogram_private_count[j] += 1;	
				}
			}
		}
		#pragma omp barrier
	
		// Add all the local counts from  the threads to the total count.	
		#pragma omp critical
		for(int i=0; i<num_bins; i++)
		{
			bc[i] += histogram_private_count[i];
		}	
	}
	mytime_t end = NOW();
	#ifdef _OPENMP
        std::cout<<"Elapsed time for histogram function with "<<num_threads<<" threads: "<<end-start<<"s"<<std::endl;
	#endif
}

int main(int argc, char *argv[])
{
	if(argc < 5)
	{
		printf("Enter \n1.Number of bins\n2.Number of elements\n3.Name of the input file\n4.Type 1 for outlier or 0 if not required\n");
		return 0;
	}
	
	nb  = atoi(argv[1]);
	int outlier = atoi(argv[4]);
	size_t n = atoi(argv[2]);
//	String fileName = argv[3]
	if(outlier ==1)
	{
		n += 1;
	}
	int j=0;
	int i=0;
	FILE *fp;
	char *line = NULL;
	size_t len_word = 0;
	ssize_t read;
	
	fp = fopen(argv[3], "r");
	if(fp == NULL)
		exit(0);
	
	double *x = new double[n];
	while(((read = getline(&line, &len_word, fp)) != -1) && (i<n))
		{
			x[i++] = atof(line);
		}
	if(outlier ==1)
	{
		x[n-1] = nb;
	}

	double *bb;
	size_t *bc;
	
	histogram(n, x, nb, &bb, &bc);
      
	for(int i=0; i<nb; ++i)
	 {
		// Uncomment thi sto print the output
		// printf("[bin %d: %5.2e --%5.2e] %d\n",i, bb[i], bb[i+1], bc[i]);
	 }
	
	//delete [] bb;
	//delete [] bc;
	fclose(fp);
	if(line)
		free(line);
	return 0;
}
