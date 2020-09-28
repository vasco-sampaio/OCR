#include <stdlib.h>
#include "nn.h"

neuralNetwork initNN(int inputs, int hiddens, int outputs)
{
	neuralNetwork nn;
	nn.inputs = inputs;
	nn.hiddens = hiddens;
	nn.outputs = outputs;
	

	layer hiddenLayer = createLayer(hiddens);
	initLayer(hiddenLayer,inputs);

	nn.hiddensTab = hiddenLayer;

	nn.inputsTab = calloc(inputs,sizeof(float));
	nn.outputsTab = calloc(outputs,sizeof(float));
	return  nn;
}

