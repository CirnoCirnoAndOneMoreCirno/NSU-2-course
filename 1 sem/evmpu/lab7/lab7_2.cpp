#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <xmmintrin.h>

#define N 16
#define M 10

using namespace std;

void sumMatrix(float* a, float* b, float* res)
{
    __m128 sum;
    for (int i = 0; i < N; ++i)
    {
        __m128* AA, * BB;
        AA = (__m128*)(a + i * N);
        BB = (__m128*)(b + i * N);
        for (int j = 0; j < N/4; j++)
        {
            sum = _mm_add_ps(AA[j], BB[j]);
            _mm_store_ps((res + i * N + 4*j), sum);
        }
    }
}

void subMatrix(float* a, float* b, float* res)
{
    __m128 sub;
    for (int i = 0; i < N; ++i)
    {
        __m128* AA, * BB;
        AA = (__m128*)(a + i * N);
        BB = (__m128*)(b + i * N);
        for (int j = 0; j < N/4; j++)
        {
                sub = _mm_sub_ps(AA[j], BB[j]);
            _mm_store_ps((res + i * N + j*4), sub);
        }
    }
}

void multMatrix(float* a, float* b, float* res)
{

    __m128 line, tmp, sum,  column, line2, resLine;
    /*
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

    */
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j+=4)
        {
            tmp = _mm_setzero_ps();
            _mm_store_ps(res + i * N + j, tmp);
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int k = 0; k < N; k++)
        {
            for (int j = 0; j < N; j+=4)
            {
                resLine = _mm_load_ps(res + i * N + j);
                //line = _mm_load_ps(a + i * N + k);
                column = _mm_set1_ps(a[i * N + k]);

                line2 = _mm_load_ps(b + k * N + j);
                tmp = _mm_mul_ps(column, line2);
                resLine = _mm_add_ps(resLine, tmp);
                _mm_store_ps(res + i * N + j, resLine);
            }
           // _mm_store_ps(res + i * N + k, sum);

        }
    }
    
    /*
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j += 4)
        {
            sum = _mm_setzero_ps();
            for (int k = 0; k < N; k++)
            {
                column = _mm_set1_ps(a[i * N + k]);
                line = _mm_load_ps(b + k * N + j);
                tmp = _mm_mul_ps(column, line);
                sum = _mm_add_ps(sum, tmp);
            }
            _mm_store_ps(res + i * N + j, sum);
        }
    }
    */
}

float getA1(float* A)
{
    float res = 0, tmp = 0;
    for (int i = 0; i < N; ++i)
    {
        tmp = 0;
        for (int j = 0; j < N; ++j)
        {
            tmp += fabsf(A[i * N + j]);
        }
        if (tmp > res)
            res = tmp;
    }
    return res;
}

float getA8(float* A)
{
    float res = 0, tmp = 0;
    for (int i = 0; i < N; ++i)
    {
        tmp = 0;
        for (int j = 0; j < N; ++j)
        {
            tmp += fabs(A[j * N + i]);
        }
        if (tmp > res)
            res = tmp;
    }
    return res;
}

float* invariateMatrix(float* A)
{
    float* B; float* I; float* BA; float* R; float* RES; float* TMP; float* TMP2;
    __m128 copyTmp;
    B = (float*)calloc(N * N, sizeof(float));
    I = (float*)calloc(N * N, sizeof(float));
    BA = (float*)calloc(N * N, sizeof(float));
    R = (float*)calloc(N * N, sizeof(float));
    RES = (float*)calloc(N * N, sizeof(float));
    TMP = (float*)calloc(N * N, sizeof(float));

    for (int i = 0; i < N; i++)
    {
        I[i * N + i] = 1;
        TMP[i * N + i] = 1;
    }

    float a1 = getA1(A);
    float a8 = getA8(A);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; j++)
        {
            B[i * N + j] = A[j * N + i] / (a8 * a1);
        }
    }

    multMatrix(B, A, BA);
    subMatrix(I, BA, R);
    for (int k = 0; k < M - 1; ++k)
    {
        multMatrix(I, R, BA);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j+=4)
            {
                copyTmp = _mm_load_ps(BA + i * N + j);
                _mm_store_ps(I + i * N + j, copyTmp);
                //I[i * N + j] = BA[i * N + j];
            }
        }
        sumMatrix(TMP, I, TMP);
    }
    multMatrix(TMP, B, RES);

    free(B);
    free(BA);
    free(I);
    free(R);
    free(TMP);
    return RES;
}

void checkRes(float* A, float* invar)
{
    float* check = (float*)calloc(N * N, sizeof(float));
    multMatrix(A, invar, check);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << check[i * N + j] << ' ';
        }
        cout << endl;
    }
    free(check);
}

int main()
{
    float* A, * invar;
    A = (float*)calloc(N * N, sizeof(float));
    float* check = (float*)calloc(N * N, sizeof(float));
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)A[i * N + j] = 25;
            else A[i * N + j] = 2;
        }
    }


    int start = clock();
    invar = invariateMatrix(A);
    int end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "t = " << t << endl;
    checkRes(A, invar);

    free(A);
    free(invar);
    return 0;
}
