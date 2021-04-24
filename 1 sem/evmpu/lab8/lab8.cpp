#include <iostream>
#include <ctime>
#include <intrin.h>
#include <vector>

#define KB 256
using namespace std;



void straightArray(unsigned int* a, unsigned int n)
{
    a[n - 1] = 0;
    for (unsigned int i = 0; i < (n - 1); i++)
    {
        a[i] = i + 1;
    }
}

void reverseArray(unsigned int* a, unsigned int n)
{
    a[0] = n - 1;
    for (unsigned int i = n - 1; i > 0; i--)
    {
        a[i] = i - 1;
    }
}

void randArray(unsigned int* a, unsigned int n)
{
    srand(time(0));
    vector<unsigned int> nums(n-1);
    for (int i = 0; i < n-1; i++)
        nums[i] = i+1;
    unsigned int index = 0;
    unsigned int prev = 0;
    for (unsigned int vecLen = n - 1; vecLen > 0; vecLen--)
    {
        index = rand() % vecLen;
        a[prev] = nums[index];
        prev = nums[index];        
        nums.erase(nums.begin() + index);
    }
    a[prev] = 0;
}

int main()
{
    volatile int k = 3;
    unsigned int n = KB * 32768;
    unsigned int* a = (unsigned int*)calloc(n, sizeof(unsigned int));

    straightArray(a, n);
    int m = 0;
    for (int i = 0; i < n * k; i++) m = a[m];
    unsigned long long int tmp, minT = 100000;
    for (int i = 0; i < 10; i++) 
    {
        m = 0;
        unsigned long long int start = __rdtsc();
        for (int i = 0; i < n * k; i++) m = a[m];
        unsigned long long int end = __rdtsc();
        tmp = (end - start) / (1ULL * n * k);
        minT = min(minT, tmp);
    }

    cout << "Straight:" << minT << endl;

    reverseArray(a, n);
    m = 0;
    for (int i = 0; i < n * k; i++) m = a[m];
    minT = 100000;
    for (int i = 0; i < 10; i++)
    {
        m = 0;
        unsigned long long int start = __rdtsc();
        for (int i = 0; i < n * k; i++) m = a[m];
        unsigned long long int end = __rdtsc();
        tmp = (end - start) / (1ULL * n * k);
        minT = min(minT, tmp);
    }

    cout << "Reverse:" << minT << endl;

    randArray(a, n);
    m = 0;
    for (int i = 0; i < n * k; i++) m = a[m];
    minT = 100000;
    for (int i = 0; i < 10; i++)
    {
        unsigned long long int start = __rdtsc();
        for (int i = 0; i < n * k; i++) m = a[m];
        unsigned long long int end = __rdtsc();
        tmp = (end - start) / (1ULL * n * k);
        minT = min(minT, tmp);
    }

    cout << "Random:" << minT << endl;

    return 0;
    
}
