

# include <stdlib.h>
# include <math.h>



double randVal(double min, double max)
{
	double r = (double) rand();
	r = r / (double) RAND_MAX * (max - min) + min;
	return r;
}


// The sigmoid function and its derivative
double sigmoid(double x) { return 1 / (1 + exp( -x )); }
double dSigmoid(double sig) { return sig * (1 - sig); }


// fixed ReLU function
double ReLU(double x)
{
	if(x < 0)
		return x * 0.01;
	return x;
}


// Apply the softmax function to an array
void softMax(double *array, int len)
{
	double sum = 0;
	for(int i = 0; i < len; ++i)
	{
		array[i] = exp(array[i]);
		sum += array[i];
	}

	for(int j = 0; j < len; ++j)
		array[j] /= sum;
}


void applyFunc(double *array, int len, double (*fct)(double))
{
	for(int i = 0; i < len; ++i)
		array[i] = fct(array[i]);
}



void resetAlloc(double *ptr, int size)
{
	free(ptr);
	ptr = calloc(size, sizeof(double));
}
