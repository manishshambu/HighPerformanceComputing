#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include "slurp_file.h"
#include<mpi.h>

using namespace std;


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Enter 1. Array file name\n");
		return 0;
	}
	//int line_num = atoi(argv[1]);
	int numtasks, rank;

	const char* fname = argv[1];
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> sub_array[numtasks];

	slurp_file_line(fname, rank, &sub_array[rank]);
	MPI_Status st;

	if(rank == 0)
	{
		int localMax = 0;
		int globalMax = 0;
		int recievedValue;
		for(int i = 0; i < 10; i++)
		{
			if(sub_array[rank][i] > localMax)
			{
				localMax = sub_array[rank][i];
			}
		}


		//printf("Local max from line %d is %d\n", rank, localMax);
		MPI_Recv(&recievedValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &st);
		if(recievedValue > localMax)
		{
			globalMax = recievedValue;
		}
		else
		{
			globalMax = localMax;
		}
		printf("The global Max is %d\n",globalMax);
	}
	else if(rank < numtasks-1)
	{
		int localMax = 0;
		int globalMax;
		int recievedValue;
		for(int i = 0; i < 10; i++)
		{
			if(sub_array[rank][i] > localMax)
			{
				localMax = sub_array[rank][i];
			}
		}

		//printf("Local max from line %d is %d\n", rank, localMax);

		MPI_Recv(&recievedValue,  1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &st);
		//printf("Recieved from %d and recieved value is %d\n",  rank+1, recievedValue);
		
		if(recievedValue > localMax)
		{
			globalMax =  recievedValue;
		}
		else
		{
			globalMax = localMax;
		}
		MPI_Send(&globalMax, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
		//printf("Sent %d local Max from processor %d\n", localMax, rank);
		
	}
	if(rank == numtasks-1)
	{
		int localMax = 0;
		for(int i = 0; i < 10; i++)
		{
			if(sub_array[rank][i] > localMax)
			{
				localMax = sub_array[rank][i];
			}
		}

		//printf("Local max from line %d is %d\n", rank, localMax);

		MPI_Send(&localMax, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
		//printf("Sent localMac %d from last processor\n", localMax);
	}

	MPI_Finalize();
	return 0;
}
