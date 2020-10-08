#include <math.h>
#include "nn.h"

//initialise a neural network
neuralNetwork initNN(int inputs, int hiddens, int outputs)
{
	neuralNetwork res;
	
	res.numInputs = inputs;
	res.numHiddens = hiddens;
	res.numOutputs = outputs;

	res.hiddenLayer = initRandMatrix(hiddens,1);
	res.outputLayer = initRandMatrix(outputs,1);

	res.hiddenLayerBias = initRandMatrix(hiddens,1);
	res.outputLayerBias = initRandMatrix(outputs,1);

	res.hiddenWeights = initRandMatrix(inputs,hiddens);
	res.outputWeights = initRandMatrix(hiddens,outputs);
	

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
