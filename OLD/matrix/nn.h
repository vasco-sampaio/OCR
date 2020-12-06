#ifndef NN_H
#define NN_H
#include "matrix.h"

typedef struct neuralNetwork_t
{
	int numInputs;
	int numHiddens;
	int numOutputs;

	//matrix of the activation
	matrix_t hiddenLayer;
	matrix_t outputLayer;
	
	//matrix of the bias
	matrix_t hiddenLayerBias;
	matrix_t outputLayerBias;

	//matrix of the weights
	matrix_t hiddenWeights;
	matrix_t outputWeights;

}neuralNetwork;

neuralNetwork initNN(int inputs, int hiddens, int outputs);
double simgoid(double x);
double dSigmoid(double x);

//calculate activation for hidden layer
void actvHidden(neuralNetwork nn,int x);
//calculate acitvation for output layer
void actvOutput(neuralNetwork nn);

#endif
