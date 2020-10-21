# include "neuralNet.h"
# include "forProp.h"


// Return the error of the output
double *deltaOut(neuralNet nn, double *expected_outputs)
{
	double *deltaOut = calloc(nn.nbOutputs, sizeof(double));
	double error;
	// Iterate through the outputs
	for(int i = 0; i < nn.nbOutputs; ++i)
	{
		// Get the difference
		error = expected_outputs[i] - nn.outputLayer.act[i];
		
		// deltaOut = error * dSig(activation)
		deltaOut[i] = error * dSigmoid(nn.outputLayer.act[i]);
	}
	return deltaOut;
}



double *deltaPrevLayer(nn_layer layer, double *delta, double *prevAct)
{
	double *deltaL = calloc(layer.prevSize, sizeof(double));
	double error;

	for(int i = 0; i < layer.prevSize; ++i)
	{
		error = 0;
		// The error of the previous layer, for each node,
		// is the weighted sum of this layer's delta
		for(int j = 0; j < layer.size; ++j)
		{
			error += delta[j] * layer.weights[j * layer.prevSize + i];
		}
		deltaL[i] = error * dSigmoid(prevAct[i]);
	}

	return deltaL;
}
