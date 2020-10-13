#include "neuron.h"
#include <stdio.h>
#include <stdlib.h>

neuron createNeuron(int nbWeights)
{
	neuron res;
	res.activation = 0.0f;
	res.weights = calloc(nbWeights,sizeof(double)); //initialize the memory for an array of size nbWeigths
	res.bias = 0.0f;

	return res;
}
