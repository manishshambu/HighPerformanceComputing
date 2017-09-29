#include <stdio.h>
#include "mpi.h"


#include <iostream>
#include <cstdlib>
#include <cassert>

#include "slurp_file.h"


void computeMatVec (std::vector<int> *v1, std::vector<int> v0, int x0)
{
	for (std::vector<int>::iterator i=v0.begin(); i!=v0.end(); i++)
	{
		v1->push_back (*i * x0);
	}
}

void addVectors (std::vector<int> *v3, std::vector<int> v1, std::vector<int> v0)
{
	for (int i=0; i<v0.size(); i++)
	{
		v3->push_back(v1[i] +v0[i]);
	}
}

int main(int argc, char *argv[])
{

	if(argc < 3)
	{
		printf("Please enter the file name followed by number of columns");
	}

	char *fileName = argv[1];
	int num_columns = atoi(argv[2]);


	int numtasks, rank;
	MPI_Init (&argc, &argv);

	std::vector<int> v1;
	std::vector<int> v2;
	std::vector<int> v3;

	MPI_Status st;

	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);


	std::vector<int> sub_array[numtasks];
	slurp_file_line(fileName, rank, &sub_array[rank]);

	std::vector<int> vectorMul;
	slurp_file_line (fileName, numtasks, &vectorMul);

	if (rank == 0)
	{
		computeMatVec (&v1, sub_array[rank], vectorMul[rank]);
#if 0
		for (std::vector<int>::iterator i=v1.begin(); i!=v1.end(); i++)
		{
			std::cout << "ELEMENT " << *i << "\n";
		}
#endif	
		//Send it to processor 1
		MPI_Send (&v1[0], num_columns, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

	}

	if ((rank > 0) && (rank < numtasks-1))
	{
		v2.resize (num_columns);

		MPI_Recv (&v2[0], num_columns, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &st);
		computeMatVec (&v1, sub_array[rank], vectorMul[rank]);
		addVectors (&v3, v1, v2);
		MPI_Send (&v3[0], num_columns, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}

	if (rank == numtasks-1)
	{
		v2.resize (num_columns);
		MPI_Recv (&v2[0], num_columns, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &st);
		computeMatVec (&v1, sub_array[rank], vectorMul[rank]);
		addVectors (&v3, v1, v2);
#if 1
		printf("Matrix Vector Multiply result\n");
		for (std::vector<int>::iterator i=v3.begin(); i!=v3.end(); i++)
		{
			std::cout << *i << "\n";
		}
#endif
	}
	MPI_Finalize();
}
