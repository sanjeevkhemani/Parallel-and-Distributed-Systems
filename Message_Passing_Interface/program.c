#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"
#define NUMSTEPS 1000000
#define PI 3.141592653589793238462643

int main(argc, argv)
int argc;
char *argv[];
{
	int procid, numprocs;
	double mypi, pi, x;
	struct timespec start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	int n = NUMSTEPS;
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	clock_gettime(CLOCK_MONOTONIC, &start);

	double step = 1.0 / (double) NUMSTEPS;
	double sum = 0.0;
	for (double i = procid + 1; i <= NUMSTEPS; i += numprocs)
	{
	    x = step * (i - 0.5);
	    sum += 4.0 / (1.0 + x*x);
	}

	mypi = step * sum;
	clock_gettime(CLOCK_MONOTONIC, &end);
	u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	printf("PI: \t\t%.25f\nError: \t\t%.25f\n", pi, fabs(pi - PI));
	printf("Elapsed Time: \t%llu nanoseconds\n", (long long unsigned int) diff);
	MPI_Finalize();

	return 0;
}
