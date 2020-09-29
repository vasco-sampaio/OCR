#ifndef NN_H
#define NN_H
#include "layer.h"

typedef struct neuralNetwork_t
{
	int inputs;
	int hiddens;
	int outputs;

	layer inputsTab; //array of inputs
	layer hiddensTab; //layer of hidden layer
	layer outputTab; //layer of outputs
}neuralNetwork;

neuralNetwork initNN(int inputs, int hiddens, int outputs); 

#endif
