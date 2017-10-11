#include <stdio.h>
#include <stdlib.h>
#include "poisson.h"
#include <cassert>
#include <math.h>
#include <string>
#include <cstddef>

/*
 * Set up the grid communicator for the Poisson problem and samples
 * the correct portion of the domain
 *
 * @comm input communicator, typically MPI_COMM_WORLD
 * @n problem size, there are n sample points along each axis
 * @grid_comm the grid communicator (output)
 * @x sample points assigned to the current MPI process based on its grid coordinates (output)
 */
void poisson_setup(MPI_Comm &comm, int n, MPI_Comm &grid_comm, std::vector<point_t> &x)
{
	int ndims  = 3;
	int dims[] = {0,0,0};
	int coords[] = {0,0,0};
	int numNodes = 0;
	int rank = 0;
	int periods[] = {0, 0, 0};

	MPI_Status st;
        MPI_Comm_size (MPI_COMM_WORLD, &numNodes);
        MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	
	if(MPI_Dims_create(numNodes, ndims,dims) != MPI_SUCCESS)
	{
		printf("Error in dims create");
		return;
	}
	
	MPI_Comm comm_cart = NULL;
	if(MPI_Cart_create(comm, ndims, dims, periods, 1, &comm_cart) != MPI_SUCCESS)
	{
		printf("Error in creating commCart");
		return;

	}
		
	if(MPI_Cart_coords(comm, rank, ndims, coords) != MPI_SUCCESS)
	{
		printf("Error in MPI Card coords");
		return;
	}

	real_t h = 1/n-1;
	
	// Distribute the points among processors.
	/*
	int k = std::cbrt(numNodes);
	real_t m = numNodes/k;
	
	for(int i=0; i<m; i++)
	{
		for(int j =0; j<m; j++)
		{
			for(int k=0; k<m; k++)
			{
				
			}
		}
	}	

	*/

}




/*
 * @grid_comm grid communicator returned by setup
 * @a value of coefficient a at points x
 * @f value of rhs at points x
 * @v candidate solution at points x
 *@res the residual norm
 */
void poisson_residual(MPI_Comm &grid_comm, std::vector<real_t> &a, std::vector<real_t> &f,std::vector<real_t> &v, real_t &res)
{


}


int main(int argc, char *argv[])
{
        int numtasks, rank;
        MPI_Init(&argc, &argv);

        if(argc < 2)
        {
                printf("Enter n along each axis\n");
        }

        int numPoints = atoi(argv[1]);

        MPI_Comm grid_comm = 0;
	MPI_Comm comm = MPI_COMM_WORLD;
        std::vector<point_t> sample_points {};
         
        poisson_setup(comm, numPoints, grid_comm, sample_points);

	if(!grid_comm)
	{
		printf("Error in setting up grid for poisson\n");
	}
	
	


        MPI_Finalize();
}   
