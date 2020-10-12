# include "utility.h"

# include <math.h>
# include <stdlib.h>

// USEFUL FUNCTIONS FOR THE NEURAL NETWORK


//Maths
double sigmoid(double x) { return 1 / (1 + exp(-x)); }

double dSigmoid(double x) { return x * (1-x); }

// Return a random value in 0-1
double uniform() { return (double) rand() / (double) RAND_MAX; }




// Swap 2 pointer's values
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// Return a shuffled indexs array of size n
int *shuffledList(int size)
{
	int *res = calloc(size, sizeof(int));

	for(int i = 0; i < size; ++i) {*(res + i) = i;}

	for(int i = size - 1; i > 0; --i)
	{
		int j = rand() % size;
		swap((res + i), (res + j));
	}

	return res;
}


