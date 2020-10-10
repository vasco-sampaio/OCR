#include <math.h>
#include "nn.h"
#include <stdio.h>

//trainongSet
double training_input[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f}};
double training_output[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

//initialise a neural network
neuralNetwork initNN(int inputs, int hiddens, int outputs)
{
	neuralNetwork res;
	
	res.numInputs = inputs;
	res.numHiddens = hiddens;
	res.numOutputs = outputs;

	res.hiddenLayer = initRandMatrix(1,hiddens);
	res.outputLayer = initRandMatrix(1,outputs);

	res.hiddenLayerBias = initRandMatrix(1,hiddens);
	res.outputLayerBias = initRandMatrix(1,outputs);

	res.hiddenWeights = initRandMatrix(inputs,hiddens); //2*2 matrix
	res.outputWeights = initRandMatrix(hiddens,outputs);//2*1 matrix
	

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

void actvHidden(neuralNetwork nn,int x)
{
	for(int i =0;i < nn.numHiddens;i++)
	{
		double res = getMatVal(nn.hiddenLayerBias,0,i);
		
		for(int j = 0;j < nn.numInputs;j++)
		{	
			res += training_input[x][j] * getMatVal(nn.hiddenWeights,j,i);
		}
		setMatVal(nn.hiddenLayer,0,i,sigmoid(res));
	}
}

void actvOutput(neuralNetwork nn)
{
	for(int i = 0;i < nn.numOutputs;i++)
	{
		double res = getMatVal(nn.outputLayerBias,0,i);	
		for(int j = 0;j < nn.numHiddens;j++)
		{
			res += getMatVal(nn.hiddenLayer,0,j) * getMatVal(nn.outputWeights,j,i);
		}
		setMatVal(nn.outputLayer,0,i,sigmoid(res));
	}
}
