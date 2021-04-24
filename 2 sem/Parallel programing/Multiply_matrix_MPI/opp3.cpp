
#include <iostream>
#include "stdlib.h"
#include "mpi.h"

using namespace std;

void sendLines(double* matrix, double* part, int n1, int n2, int rank, int* dim, int* coords, MPI_Comm MPI_GRID, MPI_Comm COMM_LINES)
{
    MPI_Status status;
    if (rank == 0)
    {
        for(int i = 1; i < dim[0]; i++)
        {
            MPI_Send(&(matrix[i * n2 * n1 / dim[0]]), n1 * n2 / dim[0], MPI_DOUBLE, dim[1] * i, 0, MPI_GRID);
        }

        for (int i = 0; i < n2 * n1 / dim[0]; i++)
            part[i] = matrix[i];
    }
    else if (coords[1] == 0)
    {
        MPI_Recv(part, n1 * n2 / dim[0], MPI_DOUBLE, 0, 0, MPI_GRID, &status);
    }
    MPI_Bcast(part, n2 * n1 / dim[0], MPI_DOUBLE, 0, COMM_LINES);
    
}

void sendColumns(double* matrix, double* part, int n1, int n2, int rank, int* dim, 
    int* coords, MPI_Comm MPI_GRID, MPI_Comm COMM_COLUMNS)
{
    MPI_Status status;
    MPI_Datatype MPI_COLUMN;
    MPI_Type_vector(n1, n2/dim[1], n2, MPI_DOUBLE, &MPI_COLUMN);
    //MPI_Type_create_resized(MPI_COLUMN, 0, 1 * sizeof(double), &MPI_COLUMN);
    MPI_Type_commit(&MPI_COLUMN);
    if (rank == 0)
    {
        for (int i = 1; i < dim[1]; i++)
        {
            MPI_Send(&(matrix[i*n2/dim[1]]), 1, MPI_COLUMN, i, 0, MPI_GRID);
        }
       
        for (int i = 0; i < n2/dim[1]; i++)
        {
            for (int j = 0; j < n1; j++) 
            {
                part[i*n1 + j] = matrix[i + j*n1];
            }
        }
    }
    else if (coords[0] == 0)
    {
        MPI_Recv(part, n1*n2/dim[1], MPI_DOUBLE, 0, 0, MPI_GRID, &status);
    }
    
    MPI_Bcast(part,  n1*n2/dim[1], MPI_DOUBLE, 0, COMM_COLUMNS);
    
    MPI_Type_free(&MPI_COLUMN);

}

void multLinesOnColumns(double* partA, double* partB, int n1, int n2, int n3, int* dim, double* partC, int rank, int* coords)
{

    // partA is (n1/dim[0]) x n2
    // partB is n2 x (n3 / dim[1])
    // partC is  (n1/dim[0]) x (n3 / dim[1])
    int l = n1 / dim[0];
    int m = n2;
    int n = n3 / dim[1];
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < m; k++)
            {
                partC[i * l + j] += partA[i * m + k] * partB[j * m + k];
            }
        }
    }
    //c[i][j] = a[i][0]*b[0][j] + a[i][1]*b[1][j] .....
    // b[0][j] == partB[j][0]
}

void collectMatrix(double* partC, double* matrix, int n1, int n2, int n3, int* dim, int rank, int* coords, int size, MPI_Comm MPI_GRID)
{
    MPI_Status status;
    MPI_Datatype MPI_BLOCK;
    MPI_Type_vector(n1 / dim[0], n3 / dim[1], n3, MPI_DOUBLE, &MPI_BLOCK);
    //MPI_Type_create_resized(MPI_BLOCK, 0, n3 / dim[1] * sizeof(double), &MPI_BLOCK);
    MPI_Type_commit(&MPI_BLOCK);
    if (rank != 0)
    {
        //printf("%d send to 0\n", rank);
        MPI_Send(partC, n1*n3 / dim[0] / dim[1], MPI_DOUBLE, 0, 0, MPI_GRID);
    }
    else
    {
        for (int i = 1; i < size; i++) 
        {
            int dispatchCoords[2] = {i/dim[1], i%dim[1]};
            int offset = dispatchCoords[0] * dim[1] * (n1 / dim[0]) * (n3 / dim[1]) + dispatchCoords[1] * (n3 / dim[1]);

            MPI_Recv(&(matrix[offset]), 1, MPI_BLOCK, i, 0, MPI_GRID, &status);
            //printf("received from %d \n", i);
        }
        for (int i = 0; i < n1 / dim[0]; i++)
        {
            for (int j = 0; j < n3 / dim[1];j++)
            {
                matrix[i * n3 + j] = partC[i * (n1 / dim[0]) + j];
            }
        }
        //for (int i = 0; i < n1 * n3; i++)
            //printf("%lf ", matrix[i]);
    }
    MPI_Type_free(&MPI_BLOCK);
}

int main(int argc, char* argv[])
{
    
    MPI_Init(&argc, &argv);               // Инициализация MPI
    double start = MPI_Wtime();

    int n1 = 4, n2 = 4, n3 = 4;
    int procCount, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount); // Получение числа процессов
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получение номера процесса


    MPI_Comm MPI_COMM_GRID;
    int dim[2] = {0};
    MPI_Dims_create(procCount, 2, dim);
    //printf("dims %d %d \n", dim[0], dim[1]);
    int coords[2] = { 0 };

    int periods[2] = {0};


    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, periods, 0, &MPI_COMM_GRID);
    MPI_Comm_rank(MPI_COMM_GRID, &rank); // Получение номера процесса

    MPI_Cart_coords(MPI_COMM_GRID, rank, 2, coords);


    MPI_Comm MPI_COMM_COLUMN, MPI_COMM_LINE;
    MPI_Comm_split(MPI_COMM_WORLD, coords[1], coords[0], &MPI_COMM_COLUMN);
    MPI_Comm_split(MPI_COMM_WORLD, coords[0], coords[1], &MPI_COMM_LINE);

    double* matrixA = NULL; double* matrixB = NULL; double* matrixC = NULL;
    double* partA = (double*)malloc(sizeof(double) * n1 * n2 / dim[0]);
    double* partB = (double*)malloc(sizeof(double) * n2 * n3 / dim[1]);
    double* partC = (double*)calloc(n1 * n3 / dim[1] / dim[0], sizeof(double));

    if (rank == 0)
    {
        matrixA = (double*)malloc(sizeof(double) * n1 * n2);
        matrixB = (double*)malloc(sizeof(double) * n2 * n3);
        matrixC = (double*)malloc(sizeof(double) * n1 * n3);
        for (int i = 0; i < n1; i++)
        {
            for (int j = 0; j < n2; j++)
            {
                matrixA[i * n2 + j] = i * n2 + j;

            }
        }
        for (int i = 0; i < n2; i++)
        {
            for (int j = 0; j < n3; j++)
            {
                matrixB[i * n2 + j] = i * n2 + j;
              
            }
        }
    }
    sendLines(matrixA, partA, n1, n2, rank, dim, coords, MPI_COMM_GRID, MPI_COMM_LINE);
    sendColumns(matrixB, partB, n2, n3, rank, dim, coords, MPI_COMM_GRID, MPI_COMM_COLUMN);
    multLinesOnColumns(partA, partB, n1, n2, n3, dim, partC, rank, coords);
    collectMatrix(partC, matrixC, n1, n2, n3, dim, rank, coords, procCount, MPI_COMM_GRID);
    double end = MPI_Wtime();
    if (rank == 0)
        printf("t = %lf\n", end - start);
    MPI_Finalize();
    return 0;
}


