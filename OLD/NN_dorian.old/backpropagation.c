# include "utility.h"
# include "backpropagation.h"

# include <stdlib.h>
# include <stdio.h>


// Update the biases and weights of the layer, depending on the delta (error) given and the learning rate
void updateLayer(double *layer, double *weights, double *biases, double *delta, int layerSize, int prevLayerSize, double learningRate)
{
	// Iterate on the layer's neurons
	for(int i = 0; i < layerSize; i++)
	{
		// First correct the bias
		*(biases + i) += *(delta + i) * learningRate;

		// Then adjust the weights 
		for(int j = 0; j < prevLayerSize; j++)
		{
			*(weights + j + i * prevLayerSize) += *(layer + i) * *(delta + i) * learningRate;
		}
	}
}


// Return the error on the output layer weights
double *outputLayerError(double *outputs, double *target, int layerSize)
{
	double *res = calloc(layerSize, sizeof(double));

	// Iterate through the neurons
	for(int i = 0; i < layerSize; i++)
	{
		// Calculate the delta
		*(res + i) = (*(target + i) - *(outputs + i)) * dSigmoid(*(outputs + i));
		//*(res + i) = dSigmoid(*(target + i) - *(outputs + i));	
		//printf("Expected output : %f\ngot : %f\ndError : %f\n",*(target + i), *(outputs + i),*(res + i));
	}

	return res;
}


// Compute the error on the hidden layer weights
double *hiddenLayerError(double *nodes, double *nextError, double *weights, int layerSize, int nextLayerSize)
{
	double *res = calloc(layerSize, sizeof(double));

	double dError;

	// Iterate through the layer neurons
	for(int i = 0; i < layerSize; i++)
	{
		dError = 0.0f;

		// Iterate throuch the following layer of the neurons, summing the weights proportionnaly to the weights connecting them
		for(int j = 0; j < nextLayerSize; j++)
		{
			//printf("modifying weight %i\n", i + j * layerSize);
			dError += *(nextError + j) * *(weights + i + j * layerSize);
		}
		*(res + i) = dError * dSigmoid(*(nodes + i));
	}
	return res;
}
