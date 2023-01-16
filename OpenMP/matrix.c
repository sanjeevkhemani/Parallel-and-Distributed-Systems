#include <omp.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2048
#define FactorIntToDouble 1.1;
#define THREADS 8

void transpose(int);
void populate_matrix(int);
double sequential(int);
double parallel(int);
double block_optimized(int);

double firstMatrix[N][N], secondMatrix[N][N], transposeMatrix[N][N], matrixMultiResult[N][N];

int main()
{
	double time;
	populate_matrix(N);
	char method;
	while(method != 'S' && method != 'P' && method != 'B')
	{
		printf("\n_________________________________________\n| MATRIX MULTIPLICATION\t\t\t|\n|---------------------------------------|\n| [Select Multiplication Method]\t|\n| S - Sequential Multiplication\t\t| \n| P - Parallel Multiplication\t\t| \n| B - Block Optimized Multiplication\t| \n|_______________________________________|\n");
		scanf("%c", &method);
	}
	switch (method)
	{
		case 'S':
			time = sequential(N);
			printf("Sequential Multiplication Calculation Time = %f seconds\n\n", time);
			break;

		case 'P':
			time = parallel(N);
			printf("Parallel Multiplication Calculation Time = %f seconds\n\n", time);
			break;

		case 'B':
			time = block_optimized(N);
			printf("Block Optimized Multiplication Calculation Time = %f seconds\n\n", time);
			break;		
	}
	return 0;
}

void transpose(int n)
{
    	for (int i = 0; i < n; i++)
	{
        	for(int j = 0; j < n; j++)
		{
            		transposeMatrix[i][j] = secondMatrix[j][i];
        	}
    	}
}

void populate_matrix(int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			firstMatrix[i][j] = (rand() % 10) * FactorIntToDouble;
			secondMatrix[i][j] = (rand() % 10) * FactorIntToDouble;
		}
	}
}

double sequential(int n)
{
	double start = omp_get_wtime();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrixMultiResult[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				matrixMultiResult[i][j] = matrixMultiResult[i][j] + firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
	double end = omp_get_wtime();
	return end - start;
}

double parallel(int n)
{
	double start = omp_get_wtime();
	#pragma omp parallel for shared(firstMatrix,  secondMatrix, matrixMultiResult) schedule(static) num_threads(THREADS)
	for (int i = 0; i < n ; i++ )
	{	
		for (int j = 0; j < n ; j++)
		{
			matrixMultiResult[i][j] = 0;
			for (int k = 0; k < n ; k++)
			{
				matrixMultiResult[i][j] = matrixMultiResult[i][j] + firstMatrix[i][k]*secondMatrix[k][j];
				
			}
		}
	}
	double end = omp_get_wtime();
	return end - start;
}

double block_optimized(int n)
{
	double start = omp_get_wtime();
	int i, j, k;
        transpose(n);
	#pragma omp parallel for shared(firstMatrix, secondMatrix, matrixMultiResult) private(i, j, k) schedule(static) num_threads(THREADS)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			double sum = 0;
			for (k = 0; k < n; k++)
			{
				sum += firstMatrix[i][k] * transposeMatrix[j][k];
			}		
			matrixMultiResult[i][j] = sum;
		}	
	}
	double end = omp_get_wtime();
	return end - start;
}