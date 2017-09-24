#include "quicksort.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#define NOW()(omp_get_wtime())
#else
#define NOW()(0)
#endif
typedef double mytime_t;
long lenArr = 0;

long partition(long *a, int low, int high)
{
	// Create 2 arrays.
	// All the elements less than pivot go to left array
	// All elements greater than pivot go to right array

	long *left = new long[high-low];
	long *right = new long[high-low];

	long i;
	long j;

	// Consider pivot as the last element in the array
	long pivot = a[high];

	long left_count =0;
	long right_count =0;

	// Elements higher than pivot in left array
	// Elements greater than pivot in right array
	for(i=low; i<high; i++)
	{
		if(a[i] < pivot)
		{
			left[left_count++] = a[i];
		}
		else
		{
			right[right_count++] = a[i];
		} 
	}

	// Copy all the values in left array to array a first
	for(i=0; i < left_count; i++)
	{
		a[low + i] = left[i];
	}

	// Insert pivot in correct position 
	a[low + left_count] = pivot;

	// Copy all the values in right array to 'a'
	for(j=0; j < right_count; j++)
	{
		a[low + left_count + j + 1] = right[j];
	}

	// return pivot position
	return low + left_count;
}

void parallelQsort(long *x, long low, long high)
{
	long pi;

	if(low < high)
	{
		pi = partition(x, low, high);

#pragma omp task
		{
			parallelQsort(x, low, pi-1);
		}

#pragma omp task
		{
			parallelQsort(x, pi+1, high);
		}

	}
}


void quicksort(const long *x, size_t n, long *y)
{
	mytime_t start = NOW(); 
	int num_threads;
#pragma omp parallel
	{ 
		num_threads = omp_get_num_threads();
#pragma omp single nowait
		parallelQsort(y, 0, n-1);
	}
	mytime_t end = NOW();

	// Uncomment this to see the output
	/*for(long i=0; i<n; i++)
	  {
	  printf("%lu\n", *(y+i));
	  }
	*/


//#ifdef _OPENMP
//	std::cout<<"Elapsed time for quicksort with "<<num_threads<<" threads: "<<end-start<<"s"<<std::endl;
//#endif
}

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Enter:\n1.Desired number of elements in array\n2.Array file name\n");
		return 0;
	}
	lenArr = atol(argv[1]);

	long *x;
	long *y;
	x = new long[lenArr];
	y = new long[lenArr];
	
	int j = 0;
	int i = 0;
	FILE *fp;
	char *line = NULL;
	size_t len_word = 0;
	ssize_t read;
	
	fp = fopen(argv[2], "r");
	if (fp == NULL)
		exit(0);


	while(((read = getline(&line, &len_word, fp)) != -1) && (i<lenArr))
	{
		x[i] = atol(line);
		y[i] = x[i];
		i++;
	}

	quicksort(x, lenArr, y);
	delete [] x;
	delete [] y;
	return 0;
}
