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
