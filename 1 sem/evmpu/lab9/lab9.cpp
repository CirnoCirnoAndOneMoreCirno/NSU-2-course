#include <iostream>
#include <intrin.h>

#define KB 256
#define ROUNDS 3

typedef unsigned int ui;
typedef unsigned long long int ull;

using namespace std;

void getArray(ui* a, ui size, int fragments, int offset)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < fragments-1; j++)
		{
			a[i + j * offset] = i + (j + 1) * offset;
		}
		a[i + (fragments - 1) * offset] = i + 1;
	}
	a[size - 1 + (fragments - 1) * offset] = 0;
}

int main()
{
	ui n = KB * 1024 * 700;
	ui* a = (ui*)calloc(n, sizeof(ui));
	for (int fragments = 1; fragments <= 32; fragments++)
	{
		getArray(a, (KB*1024*6)/fragments, fragments, KB * 1024 * 16);
		ull minT = 1000000, tmp, start, end;
		for (int ii = 0; ii < ROUNDS; ii++) 
		{
			volatile ui k = 0;
			start = __rdtsc();
			for (int jj = 0; jj < n; jj++)
				k = a[k];
			end = __rdtsc();
			tmp = (end - start) / n;
			if (tmp < minT)
				minT = tmp;
		}
		cout << "fragments: " << fragments << " ticks: " << minT << endl;
	}

}

