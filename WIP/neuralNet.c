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
	res.preAct = calloc(size, sizeof(double));
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
neuralNet initNeuralNet(int nbIn, int nbOut, int nbHidden, int *hiddenSizes)
{
	neuralNet res;
	res.nbInputs = nbIn;
	res.nbOutputs = nbOut;
	res.nbHiddenLayers = nbHidden;
	
	res.output_layer = initLayer(nbOut, hiddenSizes[nbHidden - 1]);
	res.hidden_layers = calloc(nbHidden, sizeof(nn_layer));
	res.hidden_layers[0] = initLayer(hiddenSizes[0], nbIn);

	for(int i = 1; i < nbHidden; ++i)
	{
		res.hidden_layers[i] = initLayer(hiddenSizes[i], hiddenSizes[i - 1]);
	}
	return res;
}



// Unallocate a layer
void freeLayer(nn_layer layer)
{
	free(layer.biases);
	free(layer.preAct);
	free(layer.act);
	free(layer.weights);
}

// Entirely free a neural network
void freeNeuralNet(neuralNet nn)
{
	for(int i = 0; i < nn.nbHiddenLayers; ++i)
		freeLayer(nn.hidden_layers[i]);
	freeLayer(nn.output_layer);
}
