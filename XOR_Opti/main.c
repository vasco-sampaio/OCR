#include "nn.h"
#include "train.h"
#include "neuralIO.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//debug part

double traininginputs[8] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f };
double trainingoutputs[4] = { 0.0f,1.0f,1.0f,0.0f };
double *trIn = traininginputs;
double *trOut = trainingoutputs;



int main()
{
	srand(time(NULL));
	neuralNetwork xor = initNN(2,2,1);
	train(xor, trIn, trOut, 4, 10000, 1);
	test(xor,trIn,trOut);
	writeNeuralNet("test.nn", xor);
	return 0;
}
