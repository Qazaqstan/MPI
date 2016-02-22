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

	
	struct {
		int intRank;
		double doubleRank;
	} newStruct;

	int blockLength[2];
	MPI_Aint displacements[2];
	MPI_Aint addresses[3];
	MPI_Datatype typeList[2];
	MPI_Datatype newType;

	typeList[0] = MPI_INT;
	typeList[1] = MPI_DOUBLE;

	blockLength[0] = blockLength[1] = 1;
	
	MPI_Address(&newStruct, &addresses[0]);
	MPI_Address(&(newStruct.intRank), &addresses[1]);
	MPI_Address(&(newStruct.doubleRank), &addresses[2]);

	displacements[0] = addresses[1] - addresses[0];
	displacements[1] = addresses[2] - addresses[0];
	
	MPI_Type_struct(2, blockLength, displacements, typeList, &newType);

	MPI_Type_commit(&newType);

	if (myid == 0) {
		newStruct.intRank = 100;
		newStruct.doubleRank = 1.0055;
		MPI_Send(&newStruct, 1, newType, 1, 0, MPI_COMM_WORLD);
		printf("I %d send to %d data, int = %d, double = %f\n", myid, myid + 1, newStruct.intRank, newStruct.doubleRank);
	}
	else {
		MPI_Recv(&newStruct, 1, newType, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("I %d recieve from %d data, int = %d, double = %f\n", myid, myid - 1, newStruct.intRank, newStruct.doubleRank);
	}

	MPI_Finalize();
	return 0;
}
