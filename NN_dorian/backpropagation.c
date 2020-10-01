# include "utility.h"
# include "backpropagation.h"

# include <stdlib.h>
# include <stdio.h>

void updateLayer(double *layer, double *weights, double *biases, double *delta, int layerSize, int prevLayerSize, double learningRate)
{
	for(int i = 0; i < layerSize; i++)
	{
		*(biases + i) += *(delta + i) * learningRate;
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
		*(res + i) = (*(target + i) - *(outputs + i)) * dSigmoid(*(outputs + i));
		//printf("Expected output : %f\ngot : %f\ndError : %f\n",*(target + i), *(outputs + i),*(res + i));
	}

	return res;
}


// Compute the error on the hidden layer weights
double *hiddenLayerError(double *act, double *nextError, double *weights, int layerSize, int nextLayerSize)
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
			dError += *(nextError + j) * *(weights + i + j * layerSize);
		}
		*(res + i) = dError * dSigmoid(*(act + i));
	}
	return res;
}
