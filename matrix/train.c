#include "train.h"
#include <stdio.h>
//trainingSet
double training_inputs[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
double training_outputs[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

//Calculate the error of the output
double* deltaOut(neuralNetwork nn,int x)
{
	double *deltaOutput = calloc(nn.numInputs,sizeof(double));
	for(int i = 0;i < nn.numOutputs;i++)
	{
		double error = (training_outputs[x][i] - getMatVal(nn.outputLayer,0,i));
		deltaOutput[i] = error * dSigmoid(getMatVal(nn.outputLayer,0,i));
	}
	return deltaOutput;
}

//Calculate the error of the hiddens
double* deltaHiddens(neuralNetwork nn,double* deltaO)
{
	double *deltaHidden = calloc(nn.numHiddens,sizeof(double));
	for(int i = 0;i < nn.numHiddens;i++)
	{
		double error = 0.0f;
		for(int j = 0;j < nn.numOutputs;j++)
		{
			error += deltaO[j] * getMatVal(nn.outputWeights,i,j);
		}
		deltaHidden[i] = error * dSigmoid(getMatVal(nn.hiddenLayer,0,i));
	}
	return deltaHidden;
}

//Apply changes on the weights and the bias
void applyChanges(neuralNetwork nn,double *deltaO, double *deltaH,double lr,int x)
{ 
	//Changes on the output weights and bias
	for(int i = 0;i < nn.numOutputs;i++)
	{
		setMatVal(nn.outputLayerBias,0,i,getMatVal(nn.outputLayerBias,0,i)+deltaO[i]*lr);
		for(int j = 0;j < nn.numHiddens;j++)
		{	
			double add = getMatVal(nn.outputWeights,j,i);
			setMatVal(nn.outputWeights,j,i,add + getMatVal(nn.outputLayer,0,j)*deltaO[i]*lr);
		}
	}
	//Changes on the hidden weights and bias
	for(int i = 0;i<nn.numHiddens;i++)
	{
		setMatVal(nn.hiddenLayerBias,0,i,getMatVal(nn.hiddenLayerBias,0,i)+ deltaH[i]*lr);
		for(int j = 0;j < nn.numInputs;j++)
		{
			double add2 = getMatVal(nn.hiddenWeights,j,i);
			setMatVal(nn.hiddenWeights,j,i,add2 + training_inputs[x][j]*deltaH[i]*lr);
		}
	}
}

// Swap 2 pointer's values
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// Return a shuffled indexs array of size n
int *shuffledList(int size)
{
	int *res = calloc(size, sizeof(int));

	for(int i = 0; i < size; ++i) {*(res + i) = i;}

	for(int i = size - 1; i > 0; --i)
	{
		int j = rand() % size;
		swap((res + i), (res + j));
	}

	return res;
}

void train(neuralNetwork nn)
{
	int gen = 100000;
	int nb = 0;
	double lr = 0.1;
	while(nb < gen)
	{
		int *order = shuffledList(4);
		for(int x = 0; x<4;x++)
		{
			int i = order[x];
			actvHidden(nn,i);
			actvOutput(nn);
			printf("Test with: %f and %f got %f but expect %f \n",training_inputs[x][0],training_inputs[x][1],getMatVal(nn.outputLayer,0,0),training_outputs[x][0]);
			double *deltaO = deltaOut(nn,i);
			applyChanges(nn,deltaO,deltaHiddens(nn,deltaO),lr,i);
		}
		nb++;
	}
}
