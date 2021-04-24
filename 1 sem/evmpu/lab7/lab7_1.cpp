/*
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <ctime>

using namespace std;

#define N 2048
#define M 10

float getA8(float* A) //Максимум из сумм по столбцам
{
    float res = 0.0, tmp = 0.0;
    for (int i = 0; i < N; i++)
    {
        tmp = 0.0;
        for (int j = 0; j < N; j++)
        {
            tmp += abs(A[j * N + i]);
        }
        if (tmp > res)
            res = tmp;
    }
    return res;
}

float getA1(float* A) //Максимум из сумм по строкам
{
    float res = 0, tmp = 0;
    for (int i = 0; i < N; i++)
    {
        tmp = 0;
        for (int j = 0; j < N; j++)
        {
            tmp += abs(A[i * N + j]);
        }
        if (tmp > res)
            res = tmp;
    }
    return res;
}

void subMatrix(float* a, float* b, float* res)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            res[i * N + j] = a[i * N + j] - b[i * N + j];
        }
    }
}

void sumMatrix(float* a, float* b, float* res)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            res[i * N + j] = a[i * N + j] + b[i * N + j];
        }
    }
}

void multMatrix(float* a, float* b, float* res)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            res[i * N + j] = 0;

    for (int i = 0; i < N; i++)
    {
        for (int k = 0; k < N; k++)
        {
            //res[i * N + j] = 0;
            for (int j = 0; j < N; j++)
            {
                res[i * N + j] += a[i * N + k] * b[k * N + j];//res[i][j] += a[i][k] * b[k][j]
            }
        }
    }
}

float* invariateMatrix(float* A)
{
    float* B; float* I; float* BA; float* R; float* RES; float* TMP; float* TMP2;

    B = (float*)calloc(N * N, sizeof(float));
    I = (float*)calloc(N * N, sizeof(float));
    BA = (float*)calloc(N * N, sizeof(float));
    R = (float*)calloc(N * N, sizeof(float));
    TMP = (float*)calloc(N * N, sizeof(float));
    TMP2 = (float*)calloc(N * N, sizeof(float));
    RES = (float*)calloc(N * N, sizeof(float));

    for (int i = 0; i < N; i++)
    {
        I[i * N + i] = 1.0;
        TMP[i * N + i] = 1.0;
    }

    float a8 = getA8(A);
    float a1 = getA1(A);

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            B[i * N + j] = A[j * N + i] / (a8 * a1);
        }
    }
    multMatrix(B, A, BA);
    subMatrix(I, BA, R);


    for (int k = 0; k < M - 1; ++k)
    {
        multMatrix(I, R, BA);
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                I[i * N + j] = BA[i * N + j];
            }
        }
        sumMatrix(TMP, I, TMP);
    }
    multMatrix(TMP, B, RES);


    return RES;
}

int main() {
    float* A, * invar;

    A = (float*)calloc(N * N, sizeof(float));
    float* check = (float*)calloc(N * N, sizeof(float));
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[j * N + i] = rand() % 100;
        }
    }

    int start = clock();
    invar = invariateMatrix(A);
    int end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "t = " << t << endl;
   
    free(A);
    free(invar);
    return 0;
}
*/