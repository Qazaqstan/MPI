#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int rank, size, i = 0, relay = 0, index = 0, n = 64;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double a[64] = {[0 ... 63] = 0.5};
	double start = MPI_Wtime(), startZ, stopZ;
	while (i < 50) {
	if (rank == 0) {
		if (index == 0) {
			startZ = MPI_Wtime();
			MPI_Ssend(&relay, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
			printf("0 send to 1 relay = %d\n", relay);
			MPI_Recv(&relay, n, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("0 recieve from 1 relay = %d\n", relay);
			stopZ = MPI_Wtime();
			relay++;
			index++;
		}
		else {
			MPI_Ssend(&relay, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
			printf("0 send to 1 relay = %d\n", relay);
			MPI_Recv(&relay, n, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("0 recieve from 1 relay = %d\n", relay);
			relay++;
		}
	}
	else {
		MPI_Recv(&relay, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("1 receive from 0 relay = %d\n", relay);
		MPI_Ssend(&relay, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
		printf("1 send to 0 relay = %d\n", relay);
	}
	i++;
	}
	double stop = MPI_Wtime();
	MPI_Finalize();
	if (rank == 0) {
		printf("Total time is %f\n", stop - start);
		printf("Total time is %f\n", (stop - start) * 1000);
		printf("Time for one message is %f for %d bytes\n", (n * 8.0) / (stopZ - startZ), n * 8);
		printf("Time for one message is %f for %d bytes\n", (n * 8.0) / (stopZ - startZ) * 1000.0, n * 8);
	}
	return 0;
}
