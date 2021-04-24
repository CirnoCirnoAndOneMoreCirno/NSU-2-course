#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

long double calculateFunc(long double x, long long int n)
{
    long double res = 0.0;
    char sign;
    for(long long int i = 0; i < n; ++i)
    {
        sign = (i%2 == 0)? 1:-1;
        res += (1.0L*sign*x)/(i+1);
        x*=x;
    }
    return res;
}

int main() {
    long double x, res;
    long long int n;
    cout << "Enter x and n to calculate ln(1+x)"<< endl;
    cin >> x >> n;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    res = calculateFunc(x, n);
    cout <<  setprecision(17);
    cout << res << endl;

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    cout << "Time taken: " << end.tv_sec - start.tv_sec
                              + 0.000000001*(end.tv_nsec - start.tv_nsec) << endl;
    return 0;
}

