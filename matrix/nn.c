#include <math.h>
#include "nn.h"

//initialise a neural network
neuralNetwork initNN(int inputs, int hiddens, int outputs)
{
	neuralNetwork res;
	
	res.numInputs = inputs;
	res.numHiddens = hiddens;
	res.numOutputs = outputs;

	res.hiddenLayer = initMatrix(hiddens,0);
	res.outputLayer = initMatrix(outputs,0);

	res.hiddenLayerBias = initMatrix(hiddens,0);
	res.outputLayerBias = initMatrix(outputs,0);

	res.hiddenWeights = initMatrix(inputs,hiddens);
	res.outputWeights = initMatrix(hiddens,outputs);
	

	return res;
}


//Maths
double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double dSigmoid(double x)
{
	return x * (1-x);
}
