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
# include <math.h>


static const int epochs = 10000;
static const double learningRate = 0.05;


static int nbInputs, nbHiddens, nbOutputs;

double *hiddenLayerNodes, *outputLayerNodes;
double *hiddenLayerBiases, *outputLayerBiases;
double *hiddenLayerWeights, *outputLayerWeights;




//Inits the variables of the NN
void initNeuralNetwork(int inputSize, int hiddenSize, int outputSize)
{
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

	// Initialize randomly the weights
	int i = 0;
	
	for(; i < nbInputs * nbHiddens; i++) { *hiddenLayerWeights = initWeight(); }
	
	i = 0;
	
	for(; i < nbHiddens * nbOutputs; i++) { *outputLayerWeights = initWeight(); }
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
			
			// TODO : backprop
			computeValue((inputs + index));

			// Get the error of the output layer
			double *deltaOutput = backpropOutput(outputLayerNodes, nbOutputs, expectedOutputs);

			// Get the error of the hidden layer
			double *deltaHidden = backpropLayer(hiddenLayerNodes, outputLayerWeights, deltaOutput, nbHiddens, nbOutputs);

			
		}
	}
}
