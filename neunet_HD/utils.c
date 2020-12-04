# include "utils.h"

# include <math.h>
# include <stdlib.h>

// USEFUL FUNCTIONS FOR THE NEURAL NETWORK


//Maths
double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

double dSigmoid(double sig) { return sig * (1.0 - sig); }

// Return a random value in 0-1
double uniform() { return (double) rand() / (double) RAND_MAX; }


double normal()
{
	double x1 = uniform();
	double x2 = uniform();
	double res = sqrt(-2 * log(x1)) * cos(2 * M_PI * x2);
	return res;
}


double xavier_init(int size)
{
	double val = 1 / sqrt(size);
	val = (normal() * 2 * val) - val;
	return val;
}

// return the dot product of vectors A and B, which are of size n
double dot_product(double *a, double *b, int n)
{
	double res = 0;
	for(int i = 0; i < n; ++i)
		res += a[i] * b[i];
	return res;
}

// apply a function to a whole vector
void apply_fct(double *a, double (*fct)(double), int n)
{
	for(int i = 0; i < n; ++i)
		a[i] = fct(a[i]);
}


// Swap 2 pointer's values
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// Return a shuffled indexs array of size n
void shuffledList(int size, int *dst)
{

	for(int i = 0; i < size; ++i) {dst[i] = i;}

	for(int i = size - 1; i > 0; --i)
	{
		int j = rand() % size;
		swap((dst + i), (dst + j));
	}

}
