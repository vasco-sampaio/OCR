#ifndef NN_H
#define NN_H
#include "layer.h"

typedef struct neuralNetwork_t
{
	int inputs;
	int hiddens;
	int outputs;

	float *inputsTab; //array of inputs
	layer hiddensTab; 
	float *outputsTab; //array of outputs
}neuralNetwork;

neuralNetwork initNN(int inputs, int hiddens, int outputs);

#endif
