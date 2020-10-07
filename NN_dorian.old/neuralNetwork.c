# include "neuralNetwork.h"
# include "utility.h"
# include "backpropagation.h"

# include <stdlib.h>
# include <stdio.h>
# include <time.h>



static const int epochs = 1000000;	// cycles of training
static const double lr = 0.1;		// learning rate


int nbInputNodes, nbOutputNodes;
int nbHiddenNodes;

double *outputAct, *outputBiases, *outputWeights;
double *hiddenAct, *hiddenBiases, *hiddenWeights;


// Initialize the neural network depending on the values
void initNeuralNetwork(int inputSize, int hiddenLayerSize, int outputSize)
{

	srand(time(NULL));
	// first assign all the variables and memory
	nbInputNodes = inputSize;
	nbOutputNodes = outputSize;
	nbHiddenNodes = hiddenLayerSize;

	outputAct = calloc(nbOutputNodes, sizeof(double));
	outputBiases = calloc(nbOutputNodes, sizeof(double));
	outputWeights = calloc(nbOutputNodes * nbHiddenNodes, sizeof(double));

	hiddenAct = calloc(nbHiddenNodes, sizeof(double));
	hiddenBiases = calloc(nbHiddenNodes, sizeof(double));
	hiddenWeights = calloc(nbHiddenNodes * nbInputNodes, sizeof(double));

	// Then initialize biases and weights randomly
	for(int i = 0; i < nbOutputNodes; i++)
	{
		*(outputBiases + i) = initWeight();
		for(int j = 0; j < nbHiddenNodes; j++)
		{
			*(outputWeights + i * nbHiddenNodes + j) = initWeight() + j + i;
		}
	}

	for(int i = 0; i < nbHiddenNodes; i++)
	{
		*(hiddenBiases + i) = initWeight();
		for(int j = 0; j < nbInputNodes; j++)
		{
			*(hiddenWeights + i * nbInputNodes + j) = initWeight();
		}
	}
}


// Compute the activation of a layer with a given input
void processLayer(int nbNodes, int nbIn, double *nodes, double *inputs, double *biases, double *weights)
{
	int i = 0;
	int j;
	double activation;
	for(; i < nbNodes; i++)
	{
		activation = *(biases + i);
		j = 0;
		for(; j < nbIn; j++)
		{
			activation += *(inputs + j) * *(weights + i * nbIn + j);
		}
		*(nodes + i) = sigmoid(activation);
	}
}


// Compute the output of the network with an input
void computeNetwork(double *inputs)
{
	// First process the hidden Layer
	processLayer(nbHiddenNodes, nbInputNodes, hiddenAct, inputs, hiddenBiases, hiddenWeights);
	// Then process the output
	processLayer(nbOutputNodes, nbHiddenNodes, outputAct, hiddenAct, outputBiases, outputWeights);
}


void trainNetwork(int setSize, double *trainingInputs, double *trainingOutputs)
{

	// Cycle trough the epochs
	for(int e = 0; e < epochs; e++)
	{
		int *indexes = shuffledList(setSize);


		for(int i = 0; i < setSize; i++)
		{
			int index = *(indexes + i);

			// Define clearly what set is used
			double *setInput = (trainingInputs + index * nbInputNodes);
			double *setOutput = (trainingOutputs + index * nbOutputNodes);

			// 1) Compute the output of the network
			computeNetwork(setInput);

			//printf("inputs :\n%f\n%f\ngot : %f\nexpected : %f\nerror : %f\n", *(setInput), *(setInput + 1), *(outputAct), *setOutput, (*setOutput - *outputAct));

			// 2) Compute the error between the output got and the expected one
			double *deltaOutput = outputLayerError(outputAct, setOutput, nbOutputNodes); 

			// 3) Update the weights of the output layer
			updateLayer(hiddenAct, outputWeights, outputBiases, deltaOutput, nbOutputNodes, nbHiddenNodes, lr);

			// 4) Compute the error in the hidden layer
			double *deltaHidden = hiddenLayerError(hiddenAct, deltaOutput, hiddenWeights, nbHiddenNodes, nbOutputNodes);

			// 5) Update the weights of the hidden layer
			//updateLayer(hiddenAct, outputWeights, outputBiases, deltaOutput, nbOutputNodes, nbHiddenNodes, lr);
			updateLayer(setInput, hiddenWeights, hiddenBiases, deltaHidden, nbHiddenNodes, nbInputNodes, lr);




			free(deltaOutput);
			free(deltaHidden);
		}



	}
}


void printNetwork()
{
	printf("=========== NETWORK ===========\n");
	for(int i = 0; i < nbHiddenNodes; i++)
	{
		printf("hidden Neuron %i\nbias : %f\n", i, *(hiddenBiases + i));

		for(int j = 0; j < nbInputNodes; j++)
		{
			printf("weight (%i,%i) : %f\n", j, i, *(hiddenWeights + j + i * nbInputNodes));
		}
	}
	for(int i = 0; i < nbOutputNodes; i++)
	{
		printf("output Neuron %i\nbias : %f\n", i, *(outputBiases + i));

		for(int j = 0; j < nbHiddenNodes; j++)
		{
			printf("weight (%i,%i) : %f\n", j, i, *(outputWeights + j + i * nbHiddenNodes));
		}
	}

	printf("==============================\n");
}
