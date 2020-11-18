# include "forProp.h"
# include "neuralNet.h"
# include "utils.h"

# include <stdio.h>



// Compute the activation of a layer with a given input;
void processLayer(nn_layer layer, double *inputs, double (*fct_act)(double))
{
	double res;
	for(int i = 0; i < layer.size; ++i)
	{
		res = layer.biases[i];
		for(int j = 0; j < layer.prevSize; ++j)
		{
			res += inputs[j] * layer.weights[i * layer.size + j];
		}
		layer.act[i] = fct_act(res);
	}
}



// Compute the output of the network with a given input
// TODO : Manage the no hidden layer case
void processNetwork(neuralNet nn, double *inputs)
{
	// Process the 1st layer with the actual input
	processLayer(nn.layers[0], inputs, sigmoid);

	// For each layer, process with the output of the previous one
	for(int i = 1; i < nn.nbLayers; ++i)
	{
		processLayer(nn.layers[i], nn.layers[i - 1].act, sigmoid);
	}

}
