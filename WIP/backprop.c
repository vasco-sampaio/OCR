# include "neuralNet.h"
# include "forProp.h"
# include "utils.h"

# include <stdlib.h>


// Return the error of the output
void deltaOut(nn_layer layer, double *expected_outputs, double *_delta)
{
	double error;
	// Iterate through the outputs
	for(int i = 0; i < layer.size; ++i)
	{
		// Get the difference
		error = expected_outputs[i] - layer.act[i];
		
		// deltaOut = error * dSig(activation)
		_delta[i] = error * dSigmoid(layer.act[i]);
	}
}



void deltaLayer(nn_layer layer, nn_layer nLayer, double *nextDelta, double *_delta)
{
	double error;

	for(int i = 0; i < layer.size; ++i)
	{
		error = 0;
		// The error of the previous layer, for each node,
		// is the weighted sum of this layer's delta
		for(int j = 0; j < nLayer.size; ++j)
		{
			error += nextDelta[j] * nLayer.weights[j * layer.size + i];
		}
		_delta[i] = error * dSigmoid(layer.act[i]);
	}
}



// update the given layer with the given error and inputs
void updateLayer(nn_layer layer, double *delta, double *inputs, double lr)
{
	for(int i = 0; i < layer.size; ++i)
	{
		layer.biases[i] += delta[i] * lr;
		for(int j = 0; j < layer.prevSize; ++j)
		{
			layer.weights[i * layer.size + j] +=  inputs[j] * delta[i] * lr;
		}
	}
}




