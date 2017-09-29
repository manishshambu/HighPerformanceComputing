#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int numtasks, rank, len;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);

	printf("Hello from processor %d of %d\n", rank, numtasks);
	MPI_Finalize();
}
