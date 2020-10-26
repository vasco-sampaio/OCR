# include "neuralNet.h"
# include "neuralTraining.h"
# include "forProp.h"
# include "backprop.h"
# include "utils.h"

# include <stdlib.h>


trainData trainingData(double *inputs, double *outputs)
{
	trainData res;
	res.trainInputs = inputs;
	res.trainOutputs = outputs;
	return res;
}


void trainNetwork(neuralNet nn, trainData TD, int batchsize, double lr)
{

	double *mean_error = calloc(nn.nbOutputs, sizeof(double));	
	double *inputs = TD.trainInputs;
	double *outputs = TD.trainOutputs;
	
	// Run the whole batch
	for(int i = 0; i < batchsize; ++i)
	{
		processNetwork(nn, (inputs + i * nn.nbInputs));
		deltaOut(nn, (outputs + i * nn.nbOutputs), mean_error);
	}

	// Get the average error per input
	for(int i = 0; i < nn.nbOutputs; ++i)
	{
		mean_error[i] /= batchsize;
	}


	double *error1 = mean_error;
	double *error2;

	for(int i = nn.nbLayers - 2; i >= 0; --i)
	{
		error2 = calloc(nn.layers[i].size, sizeof(double));
		deltaLayer(nn.layers[i], nn.layers[i + 1], error1, error2);
		updateLayer(nn.layers[i + 1], error1, nn.layers[i].act, lr);
		free(error1);
		error1 = error2;
	}

}
