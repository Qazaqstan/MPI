#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(argc, argv) 
int argc;
char* argv[];
{
	int myid, rank_source, rank_dest, numprocs;
	int sum = 0;
	int buffer, oldrank;
	int temp = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Status status;
	MPI_Request request;
	MPI_Comm old_comm, new_comm, sub_comm;

	int ndims, reorder;
	int dim_size[2], periods[2];
	int dims[2] = {0, 0};
	int remain_dims[2] = {1, 0};

	MPI_Dims_create( numprocs, 2, dims);

	old_comm = MPI_COMM_WORLD;
	ndims = 2;
	dim_size[0] = dims[0];
	dim_size[1] = dims[1];
	periods[0] = 1;
	periods[1] = 0;
	reorder = 0;

	MPI_Cart_create( old_comm, ndims, dim_size, periods, reorder, &new_comm);
	MPI_Comm_rank( new_comm, &oldrank);
	MPI_Cart_sub( new_comm, remain_dims, &sub_comm);
	MPI_Comm_rank( sub_comm, &myid);
	MPI_Cart_shift( sub_comm, 0, 1, &rank_source, &rank_dest);

	buffer = oldrank;

	MPI_Reduce(&buffer, &sum, 1, MPI_INT, MPI_SUM, 0, sub_comm);
	if (myid == 0) {
		printf("%d %d %d\n", oldrank, myid, sum);
	}

	MPI_Finalize();
	return 0;
}
