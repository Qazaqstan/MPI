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

	// 1ST TYPE

	struct {
		int intRank;
		double doubleRank;
	} sendStruct;

	MPI_Aint displacements[2];
	MPI_Aint addresses[3];
	MPI_Datatype sendType;
	MPI_Address(&sendStruct, &addresses[0]);
	MPI_Address(&(sendStruct.intRank), &addresses[1]);
	MPI_Address(&(sendStruct.doubleRank), &addresses[2]);
	displacements[0] = addresses[1] - addresses[0];
	displacements[1] = addresses[2] - addresses[0];
	MPI_Type_struct(2, blockLength, displacements, typeList, &sendType);
	MPI_Type_commit(&sendType);
	
	// 2ND TYPE
	
	struct {
		int intRank;
		double doubleRank;
	} recvStruct;

	MPI_Aint recvAddresses[2];
	MPI_Aint recvDisplacements[3];
	MPI_Datatype recvType;
	MPI_Address(&recvStruct, &recvAddresses[0]);
	MPI_Address(&(recvStruct.intRank), &recvAddresses[1]);
	MPI_Address(&(recvStruct.doubleRank), &recvAddresses[2]);
	recvDisplacements[0] = recvAddresses[1] - recvAddresses[0];
	recvDisplacements[1] = recvAddresses[2] - recvAddresses[0];
	MPI_Type_struct(2, blockLength, recvDisplacements, typeList, &recvType);
	MPI_Type_commit(&recvType);

	sendStruct.intRank = myid;
	sendStruct.doubleRank = myid * 1.0;

	do {
		MPI_Sendrecv(&sendStruct, 1, sendType, (myid + 1) % numprocs, 0, &recvStruct, 1, recvType, (myid - 1 + numprocs) % numprocs, 0, MPI_COMM_WORLD, &status);
		
		intSum += recvStruct.intRank;
		doubleSum += recvStruct.doubleRank;

		sendStruct.intRank = recvStruct.intRank;
		sendStruct.doubleRank = recvStruct.doubleRank;

	} while(recvStruct.intRank != myid);

	printf("process %d compute, int sum = %d, double sum = %f\n", myid, intSum, doubleSum);

	MPI_Finalize();

	return 0;
}
