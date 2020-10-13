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
	neuralNetwork xor = initNN(2,2,1);
	//train the network on 10k gens
	train(xor, trIn, trOut, 4, 50000, 1);
	//test the network for the 4possibiblite:s
	test(xor,trIn,trOut);
	return 0;
}
