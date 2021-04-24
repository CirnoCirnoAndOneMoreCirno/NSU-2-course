
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

using namespace std;

void multMatrixOnVector(double* matrix, double* vector, double* res, int N)
{
	//double* res = (double*)malloc(sizeof(double) * N);

	for (int i = 0; i < N; i++)
	{
		res[i] = 0.0;
		for (int j = 0; j < N; j++)
		{
			res[i] += matrix[i*N + j] * vector[j];
		}
	}
}

double scalarMult(double* vec1, double* vec2, int N)
{
	double res = 0.0;
	for (int i = 0; i < N; i++)
		res += vec1[i] * vec2[i];
	return res;
}

void multScalarOnVector(double* vec, double scalar, int N)
{
	for (int i = 0; i < N; i++)
		vec[i] *= scalar;
}

void subVectors(double* vec1, double* vec2, double* res, int N)
{
	//double* res = (double*)malloc(sizeof(double)*N);
	for (int i = 0; i < N; i++)
		res[i] = vec1[i] - vec2[i];
}

void printVector(double* vector, int N)
{
	for (int i = 0; i < N; i++)
		cout << vector[i] << ' ';
	cout << endl;
}

double squareRootOfScalar(double* vector, int N)
{
	double res = 0.0;
	for (int i = 0; i < N; i++)
		res += vector[i] * vector[i];
	return sqrt(res);
}

char checkSolution(double* matrix, double* vectorX, double* vectorB, int N)
{
	double eps = 0.001;
	double* tmp1 = (double*)malloc(sizeof(double) * N);
	multMatrixOnVector(matrix, vectorX, tmp1, N);
	subVectors(tmp1, vectorB, tmp1, N);
	double accuracy = (double) squareRootOfScalar(tmp1, N)/squareRootOfScalar(vectorB, N);
	free(tmp1);
	//cout << accuracy << endl;
	char res = (accuracy < eps);
	return res;
}


int solveSLE(double* matrix, double* vectorB, double* vectorX, int N)
{
	int iterations = 0;
	double* Y = (double*)malloc(sizeof(double) * N); // y = Ax^n - b
	double* tmpForTau = (double*)malloc(sizeof(double) * N); // A * y^n
	double* tmpForY = (double*)malloc(sizeof(double) * N); // Ax^n
	double* tmpForX = (double*)malloc(sizeof(double)*N); // tau^n * x^n
	double tau, tau1, tau2;
	char check = 0;
	do
	{
		multMatrixOnVector(matrix, vectorX, tmpForY, N);
		subVectors(tmpForY, vectorB, Y, N);
		multMatrixOnVector(matrix, Y, tmpForTau, N);
		tau1 = scalarMult(Y, tmpForTau, N);
		tau2 = scalarMult(tmpForTau, tmpForTau, N);
		//ne rabotaet tut
		tau = tau1 / tau2;
		multScalarOnVector(Y, tau, N);
		subVectors(vectorX, Y, vectorX, N);
		printVector(vectorX, N);
		check = checkSolution(matrix, vectorX, vectorB, N);
		iterations++;
	} while (!check);
	free(Y);
	free(tmpForTau);
	free(tmpForX);
	free(tmpForY);
	return iterations;
}

double randomDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void genMatrix(double* matrix, int N, int key)
{
	if (key == 0)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i * N + j] = 1.0;
				if (i == j)
					matrix[i * N + j] = 2.0;
			}
		}
	}
	else if (key == 1)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i * N + j] = i * N + j + 1;
			}
		}
	}
	else
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i * N + j] = randomDouble(-1.0, 1.0);
			}
		}
	}

}


int main()
{
	int N = 100;
	double* matrixA = (double*) malloc(sizeof(double*)*N*N);
	double* vectorB = (double*) malloc(sizeof(double)*N);
	double* vectorX = (double*) malloc(sizeof(double)*N);
	double* vectorU = (double*) malloc(sizeof(double) * N);

	genMatrix(matrixA, N, 1);

	

	for (int i = 0; i < N; i++)
	{
		//vectorU[i] = sin((2 * M_PI * i) / N);
		vectorU[i] = i;
		vectorX[i] = 0.0;
	}

	
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			matrixA[i*N + j] = 1.0;
			if (i == j)
				matrixA[i*N + j] = 2.0;
		}
		
	}
	
	/*
	for (int i = 0; i < N; i++)
	{
		matrixA[i] = (double*)malloc(sizeof(double) * N);
		for (int j = 0; j < N; j++)
		{
			matrixA[i][j] = (double)(rand() % 10);
		}
		vectorX[i] = 0.0;
	}
	*/
	multMatrixOnVector(matrixA, vectorU, vectorB, N);


	printVector(matrixA, N * N);
	printVector(vectorB, N);


	int it = solveSLE(matrixA, vectorB, vectorX, N);
	
	cout << it << endl;
	//printVector(vectorU, N);
	//cout << endl;
	//printVector(vectorX, N);
}