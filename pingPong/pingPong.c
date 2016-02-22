#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int rank, size, relay = 0, i = 0, index = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double start = MPI_Wtime(), startZ = 0, stopZ = 0;
	while (i < 50) {
	if (rank == 0) {
		if (index == 0) {
			startZ = MPI_Wtime();
			MPI_Send(&relay, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			printf("0 send to 1 relay = %d\n", relay);
			MPI_Recv(&relay, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("0 recieve from 1 relay = %d\n", relay);
			stopZ = MPI_Wtime();
			relay++;
			index++;
		}
		else {
			MPI_Send(&relay, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			printf("0 send to 1 relay = %d\n", relay);
			MPI_Recv(&relay, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("0 recieve from 1 relay = %d\n", relay);
			relay++;
		}
	}
	else {
		MPI_Recv(&relay, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("1 receive from 0 relay = %d\n", relay);
		MPI_Send(&relay, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		printf("1 send to 0 relay = %d\n", relay);
	}
	i++;
	}
	double stop = MPI_Wtime();
	MPI_Finalize();
	if (rank == 0) {
		printf("Total time is %f\n", stop - start);
		printf("Total time is %f\n", (stop - start) * 1000);
		printf("Time for one message is %f\n", stopZ - startZ);
		printf("Time for one message is %f\n", (stopZ - startZ) * 1000);
	}
	return 0;
}
