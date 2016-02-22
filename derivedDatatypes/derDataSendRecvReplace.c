#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(argc, argv) 
int argc;
char* argv[];
{
	int myid, numprocs;
	int intSum = 0;
	double doubleSum = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	MPI_Status status;
	MPI_Request request;

	int blockLength[2];
	MPI_Datatype typeList[2];

	typeList[0] = MPI_INT;
	typeList[1] = MPI_DOUBLE;

	blockLength[0] = blockLength[1] = 1;

	struct {
		int intRank;
		double doubleRank;
	} Struct;

	MPI_Aint displacements[2];
	MPI_Aint addresses[3];
	MPI_Datatype Type;
	MPI_Address(&Struct, &addresses[0]);
	MPI_Address(&(Struct.intRank), &addresses[1]);
	MPI_Address(&(Struct.doubleRank), &addresses[2]);
	displacements[0] = addresses[1] - addresses[0];
	displacements[1] = addresses[2] - addresses[0];
	MPI_Type_struct(2, blockLength, displacements, typeList, &Type);
	MPI_Type_commit(&Type);

	Struct.intRank = myid;
	Struct.doubleRank = myid * 1.0;

	do {
		MPI_Sendrecv_replace(&Struct, 1, Type, (myid + 1) % numprocs, 0, (myid - 1 + numprocs) % numprocs, 0, MPI_COMM_WORLD, &status);
		
		intSum += Struct.intRank;
		doubleSum += Struct.doubleRank;

	} while(Struct.intRank != myid);

	printf("process %d compute, int sum = %d, double sum = %f\n", myid, intSum, doubleSum);

	MPI_Finalize();

	return 0;
}
