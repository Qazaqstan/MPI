#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(argc, argv) 
int argc;
char* argv[];
{
	int myid, numprocs;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	int sendBuf = myid;
	int recvBuf = 0;
	int temp = 0;
	
	if (myid == 0) {
		MPI_Scan(&sendBuf, &recvBuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		printf("%d %d\n", myid, recvBuf);
		MPI_Send(&temp, 1, MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&temp, 1, MPI_INT, myid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Scan(&sendBuf, &recvBuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		printf("%d %d\n", myid, recvBuf);
		if (myid + 1 != numprocs) {
			MPI_Send(&temp, 1, MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}