#ifndef NEURON_H
#define NEURON_H
#include <stdio.h>


typedef struct neuron_t
{
	double activation;
	double *weights; //array of the weights
	double bias;

}neuron;

neuron createNeuron(int nbWeights);

#endif
