

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

