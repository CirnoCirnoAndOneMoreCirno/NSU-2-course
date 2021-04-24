
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
#include <mpi.h>
//#include <windows.h>
#include <stdlib.h>
#define ROOT 0


using namespace std;

void getColumns(double* matrix, double* cols, int N, int* linesCount, int* linesOffset, int rank)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < linesCount[rank]; j++)
        {
            cols[j*N + i] = matrix[j * N + i];
        }
    }
}

void multMatrixOnVector(double* matrix, double* vector, double* res, int N, int linesCount)
{
    //double* res = (double*)malloc(sizeof(double) * N);
    double* tmp = (double*)malloc(sizeof(double)*N);
    for (int i = 0; i < N; i++)
    {
        res[i] = 0.0;
        for (int j = 0; j < linesCount; j++)
        {
            //res[i] += (matrix[i * N + j] * vector[j]);
            tmp[i] += matrix[i * N + j] * vector[j];
        }
    }
    MPI_Allreduce(tmp, res, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    free(tmp);
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
    double eps = 0.001;
    double* tmp1 = (double*)malloc(sizeof(double) * linesCount[rank]);
    //double* totalTmp1 = (double*)malloc(sizeof(double) * N);
    double* vectorBTotal = (double*)malloc(sizeof(double) * N);
    multMatrixOnVector(partOfMatrix, vectorX, tmp1, N, linesCount[rank]);
    //MPI_Allgatherv(tmp1, linesCount[rank], MPI_DOUBLE, totalTmp1, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);

    subVectors(tmp1 + linesOffset[rank], vectorB, tmp1 + linesOffset[rank], N);
    
    MPI_Allgatherv(vectorB, linesCount[rank], MPI_DOUBLE, vectorBTotal, linesCount, linesOffset, MPI_DOUBLE, MPI_COMM_WORLD);

    double accuracy = (double)squareRootOfScalar(tmp1, N) / squareRootOfScalar(vectorB, N);

    free(tmp1);
    //free(totalTmp1);
    free(vectorBTotal);
    char res = (accuracy < eps);
    return res;
}


int solveSLE(double* partOfMatrix, double* vectorB, double* vectorX, int N,
    int procCount, int* count, int* displs, int* linesCount, int* linesOffset, int rank)
{

    cout << "check 1" << endl; 
    //printVector(vectorX, N);
    double* Y = (double*)malloc(sizeof(double) * linesCount[rank]); // y = Ax^n - b
    double* tmpForTau = (double*)malloc(sizeof(double) * N); // A * y^n
    double* tmpForY = (double*)malloc(sizeof(double) * N); // Ax^n

    double* TotalVectorX = (double*)malloc(sizeof(double) * N);
    double tau, tau1, tau2;
    int iterations = 0;
    char check = 0;
    do
    {
        multMatrixOnVector(partOfMatrix, vectorX, tmpForY, N, linesCount[rank]);
        //cout << "check 2" << endl;

        subVectors(tmpForY + linesOffset[rank], vectorB, Y, linesCount[rank]);

        //cout << "check 3" << endl;



        multMatrixOnVector(partOfMatrix, Y, tmpForTau, N, linesCount[rank]);
        //cout << "check 4" << endl;


        tau1 = scalarMult(Y, tmpForTau, N);
        tau2 = scalarMult(tmpForTau, tmpForTau, N);
        tau = tau1 / tau2;

        multScalarOnVector(Y, tau, N);
        //cout << "check 5" << endl;

        subVectors(vectorX, Y + linesOffset[rank], vectorX, linesCount[rank]);      
        //cout << "check 6" << endl;

        check = checkSolution(partOfMatrix, vectorB, vectorX, N, procCount, count, displs, linesCount, linesOffset, rank);
        iterations++;
        if (iterations > 10)
            return(10);

    } while (!check);
    free(Y);
    free(tmpForTau);
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

int main(int argc, char** argv)
{
    int N = 10;
    int procCount, rank;
    clock_t start = clock();
    MPI_Init(&argc, &argv);               // Инициализация MPI
    MPI_Comm_size(MPI_COMM_WORLD, &procCount); // Получение числа процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получение номера процесса





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


    double* matrixA = (double*)malloc(sizeof(double*) * N * N);
    //double* vectorB = (double*)malloc(sizeof(double) * N);

    double* vectorX = (double*)malloc(sizeof(double) * linesCount[rank]);
    double* vectorU = (double*)malloc(sizeof(double) * linesCount[rank]);

    //double* partOfMatrix = (double*)malloc(sizeof(double) * counts[rank]); //часть матрицы, принадлежащий данному процессу
    double* cols = (double*)malloc(sizeof(double) * N * linesCount[rank]);


    for (int i = 0; i < linesCount[rank]; i++)
    {
        vectorU[i] = sin((2 * M_PI * (i + linesOffset[rank])) / N);
        vectorX[i] = 0.0;
    }

    if (rank == ROOT)
    {
        cout << "Proccess count: " << procCount << endl;
    }
    genMatrix(matrixA, N, 0);
    getColumns(matrixA, cols, N, linesCount, linesOffset, rank);
    
    //printVector(vectorX, N);

    //MPI_Scatterv(matrixA, counts, displs, MPI_DOUBLE, partOfMatrix, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //MPI_Scatterv(vectorU, counts, displs, MPI_DOUBLE, partOfMatrix, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //MPI_Scatterv(vectorX, counts, displs, MPI_DOUBLE, partOfMatrix, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);



    int iterations = solveSLE(cols, vectorU, vectorX, N, procCount, counts, displs, linesCount, linesOffset, rank);
    cout << "====" << endl;

    clock_t end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;
    if (rank == ROOT)
    {
        // printVector(vectorU, N);
         //printVector(vectorX, N);
        cout << "t = " << t << endl;
        cout << iterations << endl;
    }
    MPI_Finalize();

}