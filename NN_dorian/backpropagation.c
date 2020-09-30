# include "utility.h"
# include "backpropagation.h"

# include <stdlib.h>

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



double *outputLayerError(double *outputs, double *target, int layerSize)
{
	double *res = calloc(layerSize, sizeof(double));

	for(int i = 0; i < layerSize; i++)
	{
		*(res + i) = (*(target + i) - *(outputs + i)) * dSigmoid(*(outputs + i));
		//printf("Expected output : %f\ngot : %f\ndError : %f\n",*(target + i), *(outputs + i),*(res + i));
	}

	return res;
}


double *hiddenLayerError(double *act, double *nextError, double *weights, int layerSize, int nextLayerSize)
{
	double *res = calloc(layerSize, sizeof(double));

	double dError;

	for(int i = 0; i < layerSize; i++)
	{
		dError = 0.0f;
		for(int j = 0; j < nextLayerSize; j++)
		{
			dError += *(nextError + j) * *(weights + i + j * layerSize);
		}
		*(res + i) = dError * dSigmoid(*(act + i));
	}
	return res;
}
