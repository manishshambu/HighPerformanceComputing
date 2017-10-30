#include "sparse.h"
#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <math.h>

typedef double data_t;


void csrToCoo(std::vector<size_t> data, std::vector<size_t> column, std::vector<size_t> row_ptr, std::vector<size_t> &x,std::vector<size_t> &y, std::vector<size_t> &value)
{
	std::vector<size_t> numElementsInRow = {};
	for(size_t i = row_ptr.size(); i > 1; i--)
	{
		numElementsInRow.push_back(row_ptr.at(i-1) - row_ptr.at(i-2));
	}
	std::reverse(numElementsInRow.begin(), numElementsInRow.end());
	for(size_t i = 0; i < data.size(); ++i) 
	{
		y.push_back(column.at(i));
		value.push_back(data.at(i)); 
	}
	for(size_t i = 0; i < numElementsInRow.size(); i++)
	{
		for(size_t j = 0; j < numElementsInRow.at(i); j++)
		{
			x.push_back(i);	
		}
	}
}

void cooToCsr(std::vector<size_t> &data, std::vector<size_t> &column, std::vector<size_t> &row_ptr, std::vector<size_t> x,std::vector<size_t> y, std::vector<size_t> value)
{
	size_t numElements = value.size();

	for(size_t i = 0; i < numElements; i++)
	{
		data.push(value[i]);
		column.push(y[i]);
	}
	
	std::map<size_t, int> rowCount;
	for(size_t i = 0; i < numElements; i++)
	{
		if(rowCount.find(x[i]) != rowCount.end())
		{
			rowCount[x[i]] += 1;
		}
		else
		{
			rowCount[x[i]] = 1;
		}
	}
	
	row_ptr[0] = 0;
	for(size_t i = 0; i < rowCount.size(); i++)
	{
		
	}
	

}

void transpose(std::vector<size_t> &x, std::vector<size_t> &y, std::vector<size_t> &value)
{
	int numtasks, rank;
	size_t numElements = value.size();
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	for(size_t i = 0; i < numElements; i++)
        {
                size_t rowNum = x.at(i);

                if( rank == floor( rowNum/ numtasks ))
                {
			size_t temp = x.at(i);
			x[i] = y.at(i);
			y[i] = temp;
                }
        }
	
	// Find the submatrix position for each row indices after transpose
	int subMatrixSize = ceil(numElements/numtasks);
	std::vector<size_t> dest_row(numElements);

	for(int i = 0; i < numElements; i++)
	{
		dest_row[i] = x[i]/subMatrixSize;
	}
	
	std::vector<size_t> index;
	sort_index(dest_row, index);
	size_t row[numElements];
	size_t col[numElements];
	size_t data[numElements];
	
	std::vector<int> counts(numtasks, 0);
	std::vector<int> displace(numtasks, 0);
	for(int i=0; i< index.size(); i++)
	{
		row[i] = x[index[i]]%subMatreixSize;
		col[i] = y[index[i]]+rank*numElements;
		data[i] = value[index[i]];
		counts[row[index[i]]] += 1;
	}

	for(int i=0; i < index.size(); i++)
	{
		displace[i] = displace[i-1]+ counts[i-1];
	}
	int recvCounts[numtasks], recvDisplace[numtasks];
	MPI_Alltoall(counts, 1, MPI_INT, recvCounts, 1, MPI_INT, MPI_COMM_WORLD);
	for(int i=0; i<numtasks; i++)
	{
		
	}

	// Initialise vectors to store the recieved row, column and data
	std::vector<size_t> new_row;
	std::vector<size_t> new_col;
	std::vector<size_t> new_data;

	MPI_Alltoallv(row, counts, x );
	MPI_Alltoallv(column, counts,y );
	MPI_Alltoallv(data, counts,value );
	
	cooToCsr();
}


int main(int argc, char *argv[])
{
	int numtasks, rank;
/*	
	int rows, columns;

	if(argc < 3)
	{
		printf("Please Enter nummber of rows followed by number of columns\n");
	}
	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
*/
	std::vector<size_t> data 	= {1, 2, 3, 4, 5, 6};
	std::vector<size_t> column 	= {0, 2, 1, 3, 2, 0};
	std::vector<size_t> row_ptr 	= {0, 2, 4, 5, 6};

//	generateCSRMatrix(rows, columns, data, column, row_ptr);
	
	std::vector<size_t> x = {};
	std::vector<size_t> y = {};
	std::vector<size_t> value = {};

	csrToCoo(data, column, row_ptr, x, y, value);

	MPI_Init (&argc, &argv);
        MPI_Status st;

        MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	transpose(x, y, value);
	
	MPI_Finalize();
}
