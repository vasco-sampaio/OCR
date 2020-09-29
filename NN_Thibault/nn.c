#include <stdlib.h>
#include "nn.h"

neuralNetwork initNN(int inputs, int hiddens, int outputs) //inputs is the number of inputs, hiddens is the number of neuron in the hidden layer and outputs is the number of outputs
{
	neuralNetwork nn;
	nn.inputs = inputs;
	nn.hiddens = hiddens;
	nn.outputs = outputs;
	

	layer inputLayer = createLayer(inputs);
	layer hiddenLayer = createLayer(hiddens);
	layer outputLayer = createLayer(outputs);
	initLayer(inputLayer,0); //the neuron in the input layer doesn't need weights
	initLayer(hiddenLayer,inputs);
	initLayer(outputLayer,hiddens);

	nn.hiddensTab = hiddenLayer;
	nn.outputTab = outputLayer;
	nn.inputsTab = inputLayer;

	
	return  nn;
}


