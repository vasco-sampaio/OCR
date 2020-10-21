# include "neuralNet.h"
# include "forProp.h"
# include "backprop.h"
# include "utils.h"








void trainNetwork(neuralNet nn, double *inputs, double *outputs, int batchsize, double lr)
{

	double *mean_error = calloc(nn.nbOutputs, sizeof(double));	
	
	// Run the whole batch
	for(int i = 0; i < batchsize; ++i)
	{
		processNetwork(nn, inputs[i * nn.nbInputs]);
		deltaOut(nn, outputs[i * nn.nbOutputs], mean_error);
	}

	// Get the average error per input
	for(int i = 0; i < nn.nbOutputs; ++i)
	{
		mean_error[i] /= batchsize;
	}

}
