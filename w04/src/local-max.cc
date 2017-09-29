#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "slurp_file.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Enter 1. Array file name\n");
		return 1;
	}
	int line_num = atoi(argv[1]);
	int numtasks, rank;

	const char* fname = argv[1];
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> array[numtasks];

	slurp_file_line(fname, rank, &array[rank]);

	int localMax = 0;
	for(int i=0; i < 10; i++)
	{
		if(array[rank][i] > localMax)
		{
			//printf("%d\n", array[rank][i]);
			localMax = array[rank][i];
		}
	}		
	printf("Local max from line %d is %d\n", rank, localMax);
	MPI_Finalize();

return 0;
}
