#include "time.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>

#define N 2048
#define M 10

using namespace std;

float getA8(float* A) //Максимум из сумм по столбцам
{
    float res = 0.0, tmp = 0.0;
    for (int i = 0; i < N; i++)
    {
        tmp = 0.0;
        for (int j = 0; j < N; j++)
        {
            tmp += fabs(A[j * N + i]);
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
            tmp += fabs(A[i * N + j]);
        }
        if (tmp > res)
            res = tmp;
    }
    return res;
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
    float* term_prev = (float*)calloc(N * N, sizeof(float));

    RES = (float*)calloc(N * N, sizeof(float));

    for (int i = 0; i < N; i++)
    {
        I[i * N + i] = 1.0;
        TMP[i * N + i] = 1.0;
    }

    float a8 = getA8(A);
    float a1 = getA1(A);
    //B = A/(...)
    cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, (float)1 / (a1 * a8), A, N, I, N, 0, B, N);

    //BA
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, B, N, A, N, 0, BA, N);

    cblas_saxpy(N * N, -1.0, BA, 1, I, 1);
    cblas_scopy(N * N, I, 1, R, 1);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, TMP, N, R, N, 0, term_prev, N);//term = I*R


    cblas_saxpy(N * N, 1.0, term_prev, 1, TMP2, 1);

    for (int i = 0; i < M - 1; i++)
    {
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, term_prev, N, R, N, 0, TMP, N); //TMP = R^i * R
        cblas_saxpy(N * N, 1.0, TMP, 1, TMP2, 1); //TMP2 = TMP + term_prev
        cblas_scopy(N * N, TMP, 1, term_prev, 1); //term_prev = TMP
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, TMP2, N, B, N, 0, RES, N);
    free(B);
    free(I);
    free(BA);
    free(TMP);
    free(TMP2);
    free(term_prev);

    return RES;
}

int main() {
    float* A = (float*)calloc(N * N, sizeof(float));
    float* inver;

    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[j * N + i] = rand() % 100;
        }
    }

    int start = clock();
    inver = invariateMatrix(A);
    int end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "t = " << t << endl;
    return 0;
}


