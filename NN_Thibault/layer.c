#include "layer.h"

layer createLayer(int nbNeuron)
{
	layer res;
	res.nbNeu = nbNeuron;
	res.neu = calloc(nbNeuron,sizeof(neuron));

	return res;
}

//initiazlize a layer with random neurons

void initLayer(layer eLayer,int nbNodePr) //nbNodePr = Number of node of the previous layer
{
	for(int i = 0; i<eLayer.nbNeu; i++)
	{
		//initalize bias and activation with random numbers between 0 and 1
		eLayer.neu[i].activation = (float) rand()/RAND_MAX;
		eLayer.neu[i].bias = (float) rand()/RAND_MAX;
		eLayer.neu[i].weights = calloc(nbNodePr,sizeof(float));

		for(int j = 0;j < nbNodePr;j++)
		{
			eLayer.neu[i].weights[j] = (float) rand() / RAND_MAX;
		}
	}
}

