# include "utility.h"
# include "nn.h"

# include <math.h>
# include <stdio.h>





//initialise a neural network
neuralNetwork initNN(int inputs, int hiddens, int outputs)
{
	neuralNetwork res;
	
	res.numInputs = inputs;
	res.numHiddens = hiddens;
	res.numOutputs = outputs;

	res.hiddenLayer = initMatrix(1,hiddens);
	res.outputLayer = initMatrix(1,outputs);

	res.hiddenLayerBias = initRandMatrix(1,hiddens);
	res.outputLayerBias = initRandMatrix(1,outputs);

	res.hiddenWeights = initRandMatrix(inputs,hiddens); //2*2 matrix
	res.outputWeights = initRandMatrix(hiddens,outputs);//2*1 matrix
	printMatrix(res.hiddenWeights);
	

	return res;
}


// FORWARD PROP

// Compute the hidden layer activation
void actvHidden(neuralNetwork nn, double *inputs)
{
	for(int i = 0; i < nn.numHiddens; i++)
	{
		double res = getMatVal(nn.hiddenLayerBias,0,i);
		
		for(int j = 0;j < nn.numInputs;j++)
		{	
			res += inputs[j] * getMatVal(nn.hiddenWeights,j,i);
		}
		setMatVal(nn.hiddenLayer,0,i,sigmoid(res));
	}
}


// Compute the output layer activation
void actvOutput(neuralNetwork nn)
{
	for(int i = 0; i < nn.numOutputs; i++)
	{
		double res = getMatVal(nn.outputLayerBias,0,i);	
		for(int j = 0; j < nn.numHiddens; j++)
		{
			res += getMatVal(nn.hiddenLayer,0,j) * getMatVal(nn.outputWeights,j,i);
		}
		setMatVal(nn.outputLayer,0,i,sigmoid(res));
	}
}
