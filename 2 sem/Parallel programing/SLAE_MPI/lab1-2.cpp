
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <mpi.h>
//#include <windows.h>
#include <stdlib.h>
#define ROOT 0


using namespace std;


void multMatrixOnVector(double* matrix, double* vector, double* res, int N, int linesCount)
{
	//double* res = (double*)malloc(sizeof(double) * N);

	for (int i = 0; i < linesCount; i++)
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
	//cout << endl << "-----" << endl;
	for (int i = 0; i < N; i++)
	{
		//cout << vec1[i] << ' ' << vec2[i] << endl;
		res += (vec1[i] * vec2[i]);
	}
	//cout <<  "----" << endl;
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

char checkSolution(double* partOfMatrix, double* vectorB, double* vectorX, int N,
	int procCount, int* count, int* displs, int* linesCount, int* linesOffset, int rank)
{
	double eps = 0.00001;
	double* tmp1 = (double*)malloc(sizeof(double) * linesCount[rank]);
	double* totalTmp1 = (double*)malloc(sizeof(double) * N);

	multMatrixOnVector(partOfMatrix, vectorX, tmp1, N, linesCount[rank]);
	MPI_Allgatherv(tmp1, linesCount[rank], MPI_DOUBLE, totalTmp1, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);
	
	subVectors(totalTmp1, vectorB, totalTmp1, N);
	double accuracy = (double)squareRootOfScalar(totalTmp1, N) / squareRootOfScalar(vectorB, N);
	free(tmp1);

	char res = (accuracy < eps);
	return res;
}


int solveSLE(double* partOfMatrix, double* vectorB, double* vectorX, int N,
	int procCount, int* count, int* displs, int* linesCount, int* linesOffset, int rank)
{
	//printVector(vectorX, N);
	double* Y = (double*)malloc(sizeof(double) * linesCount[rank]); // y = Ax^n - b
	double* tmpForTau = (double*)malloc(sizeof(double) * linesCount[rank]); // A * y^n
	double* tmpForY = (double*)malloc(sizeof(double) * linesCount[rank]); // Ax^n
	//double* tmpForX = (double*)malloc(sizeof(double) * linesCount[rank]); // tau^n * x^n

	double* TotalTmpForTau = (double*)malloc(sizeof(double) * N);	
	double* TotalY = (double*)malloc(sizeof(double) * N);
	double* TotalTmpForY = (double*)malloc(sizeof(double) * N);

	double tau, tau1, tau2;
	int iterations = 0;
	char check = 0;
	do
	{
		multMatrixOnVector(partOfMatrix, vectorX, tmpForY, N, linesCount[rank]);
		
		MPI_Allgatherv(tmpForY, linesCount[rank], MPI_DOUBLE, TotalTmpForY, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);
		
		for (int i = 0; i < linesCount[rank]; i++)
		{
			//cout << TotalTmpForY[i] << ' ';
			Y[i] = TotalTmpForY[i + linesOffset[rank]] - vectorB[i + linesOffset[rank]];
		}

		MPI_Allgatherv(Y, linesCount[rank], MPI_DOUBLE, TotalY, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);

		multMatrixOnVector(partOfMatrix, TotalY, tmpForTau, N, linesCount[rank]);


		//printVector(TotalY, N);
		MPI_Allgatherv(tmpForTau, linesCount[rank], MPI_DOUBLE, TotalTmpForTau, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);

		
		//Sleep(1000);
		//printVector(TotalY, N);
		//Sleep(1000);
		//printVector(TotalTmpForTau, N);


		tau1 = scalarMult(TotalY, TotalTmpForTau, N);
		tau2 = scalarMult(TotalTmpForTau, TotalTmpForTau, N);
		tau = tau1 / tau2;
		
		//if (rank == ROOT)
			//cout << "Debug:" << tau << endl;

		multScalarOnVector(TotalY, tau, N);
		subVectors(vectorX, TotalY, vectorX, N);
		check = checkSolution(partOfMatrix, vectorB, vectorX, N, procCount, count, displs, linesCount, linesOffset, rank);
		iterations++;
	} while (!check);

	free(Y);
	free(tmpForTau);
	free(tmpForY);
	free(TotalY);
	free(TotalTmpForTau);
	free(TotalTmpForY);
	return iterations;
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void genMatrix(double* matrix, int N, int key)
{
	if (key == 0) //генерируем матрицу из 1 и 2; для демонстрации правильности решения СЛАУ
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
	else
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix[i * N + j] = fRand(-100.0, 100.0);

			}
		}
	}
}

int main(int argc, char** argv)
{
	int N = 10;
	int procCount, rank;
	MPI_Init(&argc, &argv);               // Инициализация MPI
	MPI_Comm_size(MPI_COMM_WORLD, &procCount); // Получение числа процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получение номера процесса
	
	double* matrixA = (double*)malloc(sizeof(double*) * N * N);
	double* vectorB = (double*)malloc(sizeof(double) * N);

	double* vectorX = (double*)malloc(sizeof(double) * N);
	double* vectorU = (double*)malloc(sizeof(double) * N);

	int offset = 0;
	int* counts = (int*)malloc(sizeof(int) * procCount); //Сколько double'ов в i процессе
	int* displs = (int*)malloc(sizeof(int) * procCount); //Смещение в double'ах
	int* linesCount = (int*)malloc(sizeof(int) * procCount); //Число строк матрицы у процесса
	int* linesOffset = (int*)malloc(sizeof(int) * procCount); //Смещение в строках

	for (int i = 0; i < procCount; i++)
	{
		linesCount[i] = N / procCount;
		if (i < (N % procCount))
			linesCount[i]++;
		linesOffset[i] = offset;
		offset += linesCount[i];
		counts[i] = linesCount[i] * N;
		displs[i] = linesOffset[i] * N;
	}

	//double* partVectorB = (double*)malloc(sizeof(double) * linesCount[rank]);
	double* partOfMatrix = (double*)malloc(sizeof(double) * counts[rank]); //часть матрицы, принадлежащий данному процессу



	for (int i = 0; i < N; i++)
	{
		vectorU[i] = sin((2 * M_PI * i) / N);
		vectorX[i] = 0.0;
	}

	if (rank == ROOT) 
	{
		cout << "Proccess count: " << procCount << endl;
		//genMatrix(matrixA, N, 0);
		
		for (int i = 0; i < N; i++)
		{
			//matrixA[i] = (double*)malloc(sizeof(double) * M);
			for (int j = 0; j < N; j++)
			{
				matrixA[i * N + j] = 1.0;
				if (i == j)
					matrixA[i * N + j] = 2.0;

				//cout << matrixA[i * N + j] << ' ';
			}
			//cout << endl;
		}
		
	} 
	cout << "hi" << endl;
	MPI_Scatterv(matrixA, counts, displs, MPI_DOUBLE, partOfMatrix, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
//	multMatrixOnVector(partOfMatrix, vectorU, partVectorB, N, linesCount[rank]);

	//MPI_Allgatherv(partVectorB, linesCount[rank], MPI_DOUBLE, vectorB, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);

	//printVector(vectorB, N);

	//printVector(vectorX, N);
	int iterations = solveSLE(partOfMatrix, vectorU, vectorX, N, procCount, counts, displs, linesCount, linesOffset, rank);
	cout << "====" << endl;
	if (rank == ROOT)
	{
		printVector(vectorU, N);
		printVector(vectorX, N);
		cout << iterations << endl;
	}
	MPI_Finalize();
}


