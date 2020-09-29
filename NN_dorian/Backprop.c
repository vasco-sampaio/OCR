# include "NeuralNetwork.h"
# include "Utility.h"
# include "Backprop.h"

# include <stdlib.h>


// Compute the difference between the given output and the expected one
double *backpropOutput(double *layer, int layerSize, double *expectedOutput)
{
	double *deltaOutput = calloc(layerSize, sizeof(double));
	double dError;
	for(int i = 0; i < layerSize; i++)
	{
		dError = *(expectedOutput + i) - *(layer + i);
		*(deltaOutput + i) = dError;
	}
	return deltaOutput;
}


// Compute change to apply to internal layers
double *backpropLayer(double *layer, double *nextWeights, double *nextLayerError, int layerSize, int nextLayerSize)
{
	double *deltaLayer = calloc(layerSize, sizeof(double));
	double dError;
	int i, j;

	for(i = 0; i < layerSize; i++)
	{
		dError = 0;
		for(j = 0; j < nextLayerSize; j++)
		{
			dError = *(nextLayerError + j) * *(nextWeights + i + j * layerSize);
		}
		*(deltaLayer + i) = dError * dSigmoid(*(layer + i));	
	}
	return deltaLayer;
}


void applyBackprop(double *layer, double *delta, double *inputs, double *weights, double *biases, int layerSize, int inputSize)
