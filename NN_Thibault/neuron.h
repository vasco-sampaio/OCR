#ifndef NEURON_H
#define NEURON_H
#include <stdio.h>


typedef struct neuron_t
{
	float activation;
	float *weights; //array of the weights
	float bias;

}neuron;

neuron createNeuron(int nbWeights);

#endif
