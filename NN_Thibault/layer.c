#include "layer.h"


float sigmoid(float x )
{
	return 1 / (1+exp(-x));
}

float dSigmoid(float x) //derivative of the sigmoid
{
	return (x * (1-x));
}

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

void calcActv(layer Layer, layer previousLayer)
{
	for(int i = 0;i< Layer.nbNeu;i++)
	{
		neuron neu = Layer.neu[i];
		int sumWeights = 0;

		for(int j = 0; j<previousLayer.nbNeu;j++) //the number of weights of a neuron is the number of neuron of the previous layer
		{
			sumWeights += neu.weights[j] * previousLayer.neu[j].activation;
		}

		Layer.neu[i].activation = sigmoid(sumWeights + neu.bias);
	}
}



void printLayer(layer lay)
{
	for(int i = 0; i<lay.nbNeu;i++)
	{
		neuron neu = lay.neu[i];
		printf("Neuron %i: Activation : %f \n ",i,neu.activation);
	}
}
