#ifndef NN_H
#define NN_H
#include "matrix.h"

char alphaNum[] = {'0','1','2','3','4','5','6','7','8','9'};

typedef struct neuralNetwork_t
{
	//neuralNetwork parameters
	int numInputs;
	int numHiddens;
	int numOutputs;

	//matrix of the activation
	matrix_t hiddenLayer;
	matrix_t outputLayer;
	
	//matrix of the bias
	matrix_t hiddenBias;
	matrix_t outputBias;

	//matrix of the weights
	matrix_t hiddenWeights;
	matrix_t outputWeights;

}neuralNetwork;

neuralNetwork initNN(int inputs, int hiddens, int outputs);

//calculate activation for hidden layer
void actvHidden(neuralNetwork nn, double *inputs);
//calculate acitvation for output layer
void actvOutput(neuralNetwork nn);
//determine the output of the nn
char Output(neuralNetwork nn);
#endif
