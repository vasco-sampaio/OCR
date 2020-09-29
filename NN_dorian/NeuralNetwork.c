/*
 * This is the implementation of a basic simple hidden layer neural network
 * including backpropagation and stochastic gradient descent
 *
 * Dorian PERON ~ dorian.peron@epita.fr
 */

# include "Utility.h"
# include "NeuralNetwork.h"
# include "Backprop.h"

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


static const int epochs = 10000;
static const double learningRate = 0.1;


static int nbInputs, nbHiddens, nbOutputs;

double *hiddenLayerNodes, *outputLayerNodes;
double *hiddenLayerBiases, *outputLayerBiases;
double *hiddenLayerWeights, *outputLayerWeights;

// enable prints
int ptr = 1;

// Prints the weights and biases of the network
void printNetwork()
{
// hidden Layer
	for(int i = 0; i < nbHiddens; ++i)
	{
		printf("hidden Neuron %i:\n", i);
		printf("bias : %f\n", *(hiddenLayerBiases + i));
		
		for(int j = 0; j < nbInputs; ++j) { printf("weight (%i,%i) : %f\n", i, j, *(hiddenLayerWeights + i * nbInputs + j)); }

		printf("\n");
	}

	// output Layer
	for(int i = 0; i < nbOutputs; ++i)
	{
		printf("output Neuron %i:\n", i);
		printf("bias : %f\n", *(outputLayerBiases + i));
		
		for(int j = 0; j < nbHiddens; ++j) { printf("weight (%i,%i) : %f\n", i, j, *(outputLayerWeights + i * nbHiddens + j)); }

		printf("OUTPUT VAL: %f\n", *(outputLayerNodes + i));

		printf("\n");
	}
}

//Inits the variables of the NN
void initNeuralNetwork(int inputSize, int hiddenSize, int outputSize)
{
	// Set the rand() function's seed 
	srand(time(NULL));

	// Set the sizes of the layers
	nbInputs = inputSize;
	nbHiddens = hiddenSize;
	nbOutputs = outputSize;

	// Allocate space for the values of the weights, biases and activations
	hiddenLayerNodes = calloc(nbHiddens, sizeof(double));
	outputLayerNodes = calloc(nbOutputs, sizeof(double));
		
	hiddenLayerBiases = calloc(nbHiddens, sizeof(double));
	outputLayerBiases = calloc(nbOutputs, sizeof(double));

	hiddenLayerWeights = calloc(nbInputs * nbHiddens, sizeof(double));
	outputLayerWeights = calloc(nbHiddens * nbOutputs, sizeof(double));

	// Initialize randomly the weights and biases
	// hidden Layer
	for(int i = 0; i < nbHiddens; ++i)
	{
		*(hiddenLayerBiases + i) = initWeight();
		
		for(int j = 0; j < nbInputs; ++j)
		{
			*(hiddenLayerWeights + i * nbInputs + j) = initWeight();
		}
	}

	// output Layer
	for(int i = 0; i < nbOutputs; ++i)
	{
		*(outputLayerBiases + i) = initWeight();
		
		for(int j = 0; j < nbHiddens; ++j)
		{
			*(outputLayerWeights + i * nbHiddens + j) = initWeight();
		}
	}

	printNetwork();
}

// Compute layer's activations with given inputs
void processLayer(int layerSize, int preLayerSize, double *weights, double *biases, double *nodes, double *previousNodes)
{
	int i = 0;
	int j;
	double activation;
	
	for(; i < layerSize; ++i)
	{
		activation = *(biases + i);
		j = 0;
		
		for(; j < preLayerSize; ++j)
		{
			activation += *(previousNodes + j) * *(weights + j + i * layerSize);
		}
		*(nodes + i) = sigmoid(activation);
	}
}


// Compute the output of the neural network with a given input.
void computeValue(double *inputValues)
{
	//Process the hidden layer
	processLayer(nbHiddens, nbInputs, hiddenLayerWeights, hiddenLayerBiases, hiddenLayerNodes, inputValues);

	//Process the output layer 
	processLayer(nbOutputs, nbHiddens, outputLayerWeights, outputLayerBiases, outputLayerNodes, hiddenLayerNodes);

}

void train(int trainingSetSize, double *inputs, double *expectedOutputs)
{
	for(int n = 0; n < epochs; n++)
	{
		int *indexes = shuffledList(trainingSetSize);
		
		for(int i = 0; i < trainingSetSize; ++i)
		{
			int index = *(indexes + i);	
			
			// At first, compute the given entries
			computeValue((inputs + index * trainingSetSize));


			// Get the error of the output layer
			double *deltaOutput = backpropOutput(outputLayerNodes, nbOutputs, (expectedOutputs + index));

			// Get the error of the hidden layer
			double *deltaHidden = backpropLayer(hiddenLayerNodes, outputLayerWeights, deltaOutput, nbHiddens, nbOutputs);

			// Apply the changes to the output layer
			applyBackprop(deltaOutput, hiddenLayerNodes, outputLayerWeights, outputLayerBiases, nbOutputs, nbHiddens, learningRate);

			// Apply the changes to the hidden laye
			applyBackprop(deltaHidden, (inputs + index * trainingSetSize), hiddenLayerWeights, hiddenLayerBiases, nbHiddens, nbInputs, learningRate);

			free(deltaOutput);
			free(deltaHidden);
		}
	}
}
