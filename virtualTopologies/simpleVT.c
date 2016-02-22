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
	int buffer;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Status status;
	MPI_Request request;
	MPI_Comm old_comm, new_comm;

	int ndims, reorder;
	int dim_size[1], periods[1];

	old_comm = MPI_COMM_WORLD;
	ndims = 1;
	dim_size[0] = numprocs;
	periods[0] = 1;
	reorder = 0;

	MPI_Cart_create( old_comm, ndims, dim_size, periods, reorder, &new_comm);
	MPI_Comm_rank( new_comm, &myid);
	MPI_Cart_shift( new_comm, 0, 1, &rank_source, &rank_dest);

	buffer = myid;

	do {
		sum += buffer;
		MPI_Sendrecv_replace(&buffer, 1, MPI_INT, rank_dest, 0, rank_source, 0, new_comm, &status);
	} while (buffer != myid);
	printf("Rank %d, sum %d\n", myid, sum);
	MPI_Finalize();
	return 0;
}
