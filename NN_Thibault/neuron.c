#include "neuron.h"
#include <stdio.h>
#include <stdlib.h>

neuron createNeuron(int nbWeights)
{
	neuron res;
	res.activation = 0;
	res.weights = calloc(nbWeights,sizeof(float)); //initialize the memory for an array of size nbWeigths
	res.bias = 0;

	return res;
}
