# include "neuralNet.h"
# include "utils.h"

# include <stdlib.h>
# include <time.h>



nn_layer initLayer(int size, int prevSize)
{
	nn_layer res;
	res.size = size;
	res.prevSize = prevSize;
	
	res.biases = calloc(size, sizeof(double));
	res.act = calloc(size, sizeof(double));

	res.weights = calloc(size * prevSize, sizeof(double));

	for(int i = 0; i < size; ++i)
	{
		res.biases[i] = randVal(0, 1);
		for(int j = 0; j < prevSize; ++j)
		{
			res.weights[i * prevSize + j] = randVal(0, 1);
		}
	}
	return res;
}

// Initialize a neural network
neuralNet initNeuralNet(int nbIn, int nbLayers, int *layerSizes)
{
	neuralNet res;
	res.nbInputs = nbIn;
	res.nbOutputs = layerSizes[nbLayers - 1];
	res.nbLayers = nbLayers;
	
	res.layers = calloc(nbLayers, sizeof(nn_layer));
	res.layers[0] = initLayer(layerSizes[0], nbIn);

	for(int i = 1; i < nbLayers; ++i)
	{
		res.layers[i] = initLayer(layerSizes[i], layerSizes[i - 1]);
	}
	return res;
}



// Unallocate a layer
void freeLayer(nn_layer layer)
{
	free(layer.biases);
	free(layer.act);
	free(layer.weights);
}

// Entirely free a neural network
void freeNeuralNet(neuralNet nn)
{
	for(int i = 0; i < nn.nbLayers; ++i)
		freeLayer(nn.layers[i]);
}
