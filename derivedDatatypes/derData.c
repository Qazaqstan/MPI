#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(argc, argv) 
int argc;
char* argv[];
{
	int myid, numprocs;
	int sum = 0;
	int bufferSend, bufferRecv;
	
	MPI_Status status;
	MPI_Request request;
	
	struct {
		int myInt;
		double myDouble;
	} newtype;
	
	int block_lengths[2];
	MPI_Datatype typelist[2];
	MPI_Aint displacement[2];
	MPI_Aint addresses[3];
	
	typelist[0] = MPI_INT;
	typelist[1] = MPI_DOUBLE;

	block_lengths[0] = block_lengths[1] = 1;

	MPI_Address(newtype, &addresses[0]);
	MPI_Address(&(newtype.myInt), &addresses[1]);
	MPI_Address(&(newtype.myDouble), &addresses[2]);

	displacement[0] = addresses[1] - addresses[0];
	displacement[1] = addresses[2] - addresses[0];
	displacement[2] = addresses[3] - addresses[0];

	MPI_Type_struc(
		2, 
		block_lengths, 
		displacement, 
		typelist, 
		Message_type_ptr);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	request = MPI_REQUEST_NULL;
	bufferSend = myid;
	do {
		MPI_Isend(&bufferSend, 1, MPI_INT, (myid + 1) % numprocs, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(&bufferRecv, 1, MPI_INT, (myid - 1 + numprocs) % numprocs, 0, MPI_COMM_WORLD, &status);
		sum += bufferRecv;
		bufferSend = bufferRecv;
		MPI_Wait(&request, &status);
	} while(bufferRecv != myid);
	printf("process %d compute, sum = %d\n", myid, sum);
	MPI_Finalize();
	return 0;
}
