#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define NUMSTEPS 1000000

void swap(int *v, int i, int j)
{
	int temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

int * partition(int *v1, int n1, int *v2, int n2)
{
	int* result = (int *)malloc((n1 + n2) * sizeof(int));
	int i = 0, j = 0, k = 0;

	while(i < n1 && j < n2)
		if(v1[i] < v2[j])
		{
			result[k] = v1[i];
			k++;
			i++;
		}
		else
		{
			result[k] = v2[j];
			k++;
			j++;
		}
	if(i == n1)
		while(j < n2)
		{
			result[k] = v2[j];
			k++;
			j++;
		}
	else
		while(i < n1)
		{
			result[k] = v1[i];
			k++;
			i++;
		}
	return result;
}

void quicksort(int *v, int left, int right)
{
	int last;
	if(left >= right)
		return;
	swap(v, left, (left + right) / 2);
	last = left;
	for(int i = left + 1; i <= right; i++)
		if(v[i] < v[left])
			swap(v, ++last, i);
	swap(v, left, last);
	quicksort(v, left, last-1);
	quicksort(v, last+1, right);
}

void main(int argc, char **argv)
{
	int *data, *part, *other;
	int procid, numprocs, s, m, step;
	struct timespec start, end;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	int n = NUMSTEPS;

	if(procid == 0)
	{
		int r;
		s = n / numprocs;
		r = n % numprocs;
		data = (int *)malloc((n + s - r) * sizeof(int));
		FILE *inFile;
    		inFile = fopen("data.txt", "r");
		if (inFile == NULL)
		{
			printf("Error Reading File 'data/txt'\n");
			exit (0);
		}
		else
			printf("Data read from 'data/txt'\n");
			
		for(int i = 0; i < n; i++)
			fscanf(inFile, "%d ", &data[i]);
		if(r != 0)
		{
			for(int i = n; i < (n + s - r); i++)
				data[i] = 0;
			s = s + 1;
		}
		fclose(inFile);
	}
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	part = (int *)malloc(s * sizeof(int));
	MPI_Scatter(data, s, MPI_INT, part, s, MPI_INT, 0, MPI_COMM_WORLD);
	clock_gettime(CLOCK_MONOTONIC, &start);

	quicksort(part, 0, s - 1);
	step = 1;
	while(step < numprocs)
	{
		if(procid % (2 * step) == 0 && procid + step < numprocs)
		{
			MPI_Recv(&m, 1, MPI_INT, procid + step, 0, MPI_COMM_WORLD, &status);
			other = (int *)malloc(m * sizeof(int));
			MPI_Recv(other, m, MPI_INT, procid + step, 0, MPI_COMM_WORLD, &status);
			part = partition(part, s, other, m);
			s = s + m;
		}
		else
		{
			int near = procid - step;
			MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
			MPI_Send(part, s, MPI_INT, near, 0, MPI_COMM_WORLD);
			break;
		}
		step = step * 2;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	if(procid == 0)
	{
		FILE * outFile;
		outFile = fopen("data_sorted.txt", "w");
		for(int i = 0; i < s; i++)
			fprintf(outFile, "%d ", part[i]);
		fclose(outFile);
	}
	
	u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Elapsed Time: \t%llu nanoseconds\nSorted data saved to 'data_sorted.txt'\n", (long long unsigned int) diff);

	MPI_Finalize();
}

