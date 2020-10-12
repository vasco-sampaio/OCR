#include <stdio.h>
#include "nn.h"
#include <time.h>
#include <stdlib.h>
#include "train.h"
//debug part

double traininginputs[8] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f };
double trainingoutputs[4] = { 0.0f,1.0f,1.0f,0.0f };
double *trIn = traininginputs;
double *trOut = trainingoutputs;



int main()
{
	srand(time(NULL));
	neuralNetwork test = initNN(2,2,1);
	printf("Bias: %f \n",getMatVal(test.hiddenWeights,0,1));
	train(test, trIn, trOut, 4, 10000, 1);
	return 0;
}
