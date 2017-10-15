#include <stdio.h>
#include <stdlib.h>
#include "poisson.h"
#include <cassert>
#include <math.h>
#include <string>
#include <cstddef>
#include <cmath>
#include <ctime>

#define PI 3.1415
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
	int dims[] = {0, 0, 0};
	int coords[] = {0, 0, 0};
	int numNodes = 0;
	int rank = 0;
	int periods[] = {0, 0, 0};

	MPI_Status st;
	MPI_Comm_size (MPI_COMM_WORLD, &numNodes);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 

	//printf("Printing Rank %d", rank);
	if(MPI_Dims_create(numNodes, ndims, dims) != MPI_SUCCESS)
	{
		printf("Error in dims create\n");
		return;
	}

	if(MPI_Cart_create(comm, ndims, dims, periods, 1, &grid_comm) != MPI_SUCCESS)
	{
		printf("Error in creating commCart\n");
		return;

	}

	if(MPI_Cart_coords(grid_comm, rank, ndims, coords) != MPI_SUCCESS)
	{
		printf("Could not get the cartesian coordinates for the processor\n");
		return;
	}
	//printf("Cart coords for Proc = %d are %d, %d, %d\n", rank, coords[0], coords[1], coords[2]);

	//printf("%d %d %d", coords[0], coords[1], coords[2]);

	// Distribute the points among processors.

	//int k = std::pow(numNodes, 1/3.);

	//real_t m = n/k;
	real_t h = 1.0/(double(n) - 1.0);
	real_t iRange = n/dims[0];
	real_t jRange = n/dims[1];
	real_t kRange = n/dims[1];
	//printf("dims0 %d dims1 %d dims2 %d\n", dims[0], dims[1], dims[2]);
	//printf("Num Nodes %d\n", numNodes);
	//printf("H value %f\n", h);
	//printf("m value = %f\n", m);

	for(int i= 0 ; i < iRange; i++)
	{
		for(int j = 0; j < jRange; j++)
		{
			for(int k = 0; k < kRange; k++)
			{
				point_t point;
				point.x = (coords[0] * dims[0] + i);
				point.y = (coords[1] * dims[1] + j);
				point.z = (coords[2] * dims[2] + k);
				x.push_back(point);
				//printf("%f %f %f Rank = %d\n", point.x, point.y, point.z, rank);			
			}
		}
	}	
}



/*
 * @grid_comm grid communicator returned by setup
 * @a value of coefficient a at points x
 * @f value of rhs at points x
 * @v candidate solution at points x
 *@res the residual norm
 */
real_t poisson_residual(MPI_Comm &grid_comm, std::vector<real_t> &a, std::vector<real_t> &f,std::vector<real_t> &v, real_t &res, std::vector<point_t> &x)
{
	int ndims  = 3;
	int dims[] = {0, 0, 0};
	int coords[] = {0, 0, 0};
	int periods[] = {0, 0, 0};

	MPI_Status st;
	int rank, numProcs;
	MPI_Comm_rank(grid_comm, &rank);
	MPI_Comm_size(grid_comm, &numProcs);

	if(MPI_Cart_coords(grid_comm, rank, ndims, coords) != MPI_SUCCESS)
	{
		printf("Could not get the cartesian coordinates for the processor while calculating residual\n");
		//return;
	}
	if(MPI_Cart_get(grid_comm, ndims, dims, periods, coords) != MPI_SUCCESS)
	{
		printf("Could not retrieve the cartesian topology\n");
	}

	real_t iRange = x[x.size() - 1].x - x[0].x;
	real_t jRange = x[x.size() - 1].y - x[0].y;
	real_t kRange = x[x.size() - 1].z - x[0].z;

	real_t h = 1.0/(double(x.size() * numProcs) - 1.0);


	//printf(" value of %20.18f", v[67]);
	//	printf("iRange %f\n", iRange);
	for(real_t i = 0; i < iRange; i++)
	{
		for(real_t j = 0; j < jRange; j++)
		{
			for(real_t k = 0; k < kRange; k++)
			{
				real_t Lu = 0;
				int index = getIndex(i, j, k, iRange, jRange);
				//printf("Index %d\n", index);
				real_t v_val = v[index];
				real_t f_val = f[index];
				//printf("fVal %20.18f\n", v_val);
				real_t a_val = a[index];

				real_t part1 = a_val * v_val;
				real_t part2;

				if(i == 0)
				{
					part2 += getValFromCartShift(grid_comm, 0, -1, v_val);
					part2 += v[getIndex(i+1, j, k, iRange, jRange)] - v_val; 
				}	
				else if(i == iRange - 1)
				{
					part2 += getValFromCartShift(grid_comm, 0, 1, v_val );
					part2 += v[getIndex(i-1, j, k, iRange, jRange)] - v_val;	
				}
				else
				{
					part2 += v[getIndex(i+1, j, k, iRange, jRange)] - v_val + v[getIndex(i-1, j, k, iRange, jRange)] - v_val;
				}

				if(j == 0)
				{
					part2 += getValFromCartShift(grid_comm, 1, -1, v_val);
					part2 += v[getIndex(i, j+1, k, iRange, jRange)] - v_val;
				}
				else if(j == jRange - 1)
				{
					part2 += getValFromCartShift(grid_comm, 1, 1, v_val);
					part2 += v[getIndex(i, j-1, k, iRange, jRange)] - v_val;
				}
				else
				{
					part2 += v[getIndex(i, j+1, k, iRange, jRange)] - v_val  + v[getIndex(i, j-1, k, iRange, jRange)] - v_val;
				}

				if(k == 0)
				{
					part2 += getValFromCartShift(grid_comm, 2, -1, v_val);
					part2 += v[getIndex(i, j, k+1, iRange, jRange)] - v_val;
				}
				else if(k == kRange - 1)
				{	
					part2 += getValFromCartShift(grid_comm, 2, 1, v_val);
					part2 += v[getIndex(i, j, k-1, iRange, jRange)] - v_val;
				}
				else
				{
					part2 += v[getIndex(i, j, k+1, iRange, jRange)] - v_val + v[getIndex(i, j, k-1, iRange, jRange)] - v_val;
				}

				part2 = part2/(h*h);
				Lu = part1 - part2;

				real_t r = f_val - Lu;

				res += (r * r);

			}
		}
	}
	//printf("Residual for Proc %d %f\n",rank, res);
	//res = std::pow(residual, 1/2.);
	return res;

}

real_t getValFromCartShift(MPI_Comm &grid_comm, int direction, int  displacement, real_t &v)
{
	int sr, dr;
	if(MPI_Cart_shift(grid_comm, direction, displacement,  &sr, &dr) != MPI_SUCCESS)
	{
		printf("Error in MPI_cart_shift\n");
		return 0;
	}	

	if(dr != MPI_PROC_NULL)
	{
		MPI_Send(&v, 1, MPI_DOUBLE, dr, 0, grid_comm);
	}
	//printf("Sending values from proc %d, to proc %d\n", sr, dr);
	if(MPI_Cart_shift(grid_comm, direction , displacement * -1, &sr, &dr) != MPI_SUCCESS)
	{
		printf("Error in MPI_cart_shift\n");
	}

	//printf("Sending values from proc %d, to proc %d\n", sr, dr);
	MPI_Status st;
	if(dr != MPI_PROC_NULL)
	{
		real_t recievedVal;
		MPI_Recv(&recievedVal, 1, MPI_DOUBLE, dr, 0, grid_comm, &st);
		//printf("Printing recieved val %f\n", recievedVal);
		//printf("Printing from cart shift %20.18f\n", v);
		return recievedVal - v;
	}
	return 0;
}


int getIndex(real_t i, real_t j, real_t k, real_t iRange,real_t jRange)
{
	//printf("Printing index i = %f\n", k+ j*range + i*range*range);
	return (int)(k + j*jRange + i*iRange*jRange);
}

real_t u_func(real_t x, real_t y, real_t z)
{
//	printf("%20.18f", (sin(4 * PI * x) * sin(10 * PI * y) * sin(14 * PI * z)));
	return (sin(4 * PI * x) * sin(10 * PI * y) * sin(14 * PI * z));
}

real_t f_func(real_t x, real_t y, real_t z)
{
	return (sin(4 * PI * x) * sin(10* PI * y) * sin(14 * PI * z ) * (12 + (312 * PI * PI)));
}

int main(int argc, char *argv[])
{
	real_t finalRes = 0;
	std::clock_t start;
	start = std::clock();
	MPI_Init(&argc, &argv);
	int numNodes, rank;

	if(argc < 2)
	{
		printf("Enter n along each axis\n");
	}

	int numPoints = atoi(argv[1]);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm grid_comm = 0;
	MPI_Comm comm = MPI_COMM_WORLD;
	std::vector<point_t> sample_points {};

	//printf("Calling Poisson setup for Proc = %d\n", rank);
	poisson_setup(comm, numPoints, grid_comm, sample_points);

	if(!grid_comm)
	{
		printf("Error in setting up grid for poisson\n");
	}

	MPI_Status st;
	MPI_Comm_size (MPI_COMM_WORLD, &numNodes);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
	int cartCoords[] = {0, 0, 0};
	MPI_Cart_coords(grid_comm, rank, 3, cartCoords);

	real_t a = 12.0;

	std::vector<real_t> v_arr;
	std::vector<real_t> f_arr;
	std::vector<real_t> a_arr;

	real_t h = 1.0/(double(sample_points.size() * numNodes) - 1.0);
	//printf("Sample Points size is %d", sample_points.size());
	
	for(int i = 0; i < sample_points.size(); i++)
	{	
		real_t x = (sample_points[i].x) * h;
		real_t y = (sample_points[i].y) * h;
		real_t z = (sample_points[i].z) * h;
		//printf("%f %f %f Rank = %d\n", x, y, z, rank);
		//printf("ufunct %20.18f\n", u_func(x, y, z));
		v_arr.push_back(u_func(x, y, z));
		f_arr.push_back(f_func(x, y, z));
		a_arr.push_back(a);
	}

	//printf("Value of v[300] is %20.18f", v_arr[360]);

	// Calculate the residual for each processor.
	real_t res = 0;
	res = poisson_residual(grid_comm, a_arr, f_arr, v_arr, res, sample_points);

	// Collect res from all the procs, add it and take square root
	
	// Wait for all processors to complete calculating res
	MPI_Barrier(grid_comm);

	//printf("RES for proc = %d is %f\n", rank, res);
	MPI_Reduce(&res, &finalRes, 1, MPI_DOUBLE, MPI_SUM, 0,grid_comm);
	if(rank == 0)
	{
		//printf("Calculating final residual\n");
		finalRes = std::pow(finalRes, 1/2.);
	//	MPI_Reduce(&res, &finalRes, 1, MPI_DOUBLE, MPI_SUM, 0,grid_comm);
		printf("Final RES is %f\n", finalRes);
		
		std::clock_t stop;
		stop = std::clock();
		double time = (stop - start)/double(CLOCKS_PER_SEC);
		printf("Total time taken for numProcs = %d is %f\n", numNodes, time);
	}		

	MPI_Finalize();
}   
