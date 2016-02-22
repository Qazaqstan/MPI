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
	
	int inBuf = myid;
	int resultBuf = 0;
	MPI_Allreduce(&inBuf, &resultBuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	printf("%d %d\n", myid, resultBuf);
	MPI_Finalize();
	return 0;
}
