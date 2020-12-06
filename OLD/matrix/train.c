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
		double error = 0;
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
		setMatVal(nn.outputLayerBias,0,i,getMatVal(nn.outputLayerBias,0,i) + (deltaO[i]*lr));
		for(int j = 0;j < nn.numHiddens;j++)
		{	
			double add = getMatVal(nn.outputWeights,j,i);
			setMatVal(nn.outputWeights,j,i,add + getMatVal(nn.hiddenLayer,0,j)*deltaO[i]*lr);
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
	int gen = 10000;
	int nb = 0;
	double lr = 1;
	while(nb < gen)
	{
		int *order = shuffledList(4);
		
		for(int x = 0; x<4;x++)
		{
			int y = order[x];
			/*actvHidden(nn,y);
			actvOutput(nn);
			double deltaOutput[1] = {1};
			for(int i = 0; i< nn.numOutputs;i++)
			{
				double error = training_outputs[y][i] - getMatVal(nn.outputLayer,0,i);
				deltaOutput[i] = error*dSigmoid(getMatVal(nn.outputLayer,0,i));
			}

			double deltaHiddens[2] = {0,1};
			for(int i = 0;i < nn.numHiddens;i++)
			{
				double error = 0.0f;
				for(int j = 0;j < nn.numOutputs;j++)
				{
					error+=deltaOutput[j]*getMatVal(nn.outputWeights,j,i);

				}
				deltaHiddens[i] = error * dSigmoid(getMatVal(nn.hiddenLayer,0,i)); 
				}

			
			for(int i = 0;i < nn.numOutputs;i++)
			{
				double outputBias = getMatVal(nn.outputLayerBias,0,i);
				setMatVal(nn.outputLayerBias,0,i,outputBias + (deltaOutput[i] * lr));
				for(int j = 0;j < nn.numHiddens;j++)
				{
					double outputWeigth = getMatVal(nn.outputWeights,j,i);
					setMatVal(nn.outputWeights,j,i,outputWeigth + (getMatVal(nn.hiddenLayer,0,j)*deltaOutput[i] * lr));
				}
			}

			for(int i = 0;i < nn.numHiddens;i++)
			{
				double hiddenBias = getMatVal(nn.hiddenLayerBias,0,i);
				setMatVal(nn.hiddenLayerBias,0,i,hiddenBias + deltaHiddens[i] * lr);
				for(int j = 0;j < nn.numInputs;j++)
				{
					double hiddenWeight = getMatVal(nn.hiddenWeights,j,i);
					setMatVal(nn.hiddenWeights,j,i,hiddenWeight + (training_inputs[y][j] * deltaHiddens[i] * lr));
				}
			}*/
			  
			//int i = order[x];
			actvHidden(nn,y);
			actvOutput(nn);
			printf("Test with: %f and %f got %f but expect %f \n",training_inputs[y][0],training_inputs[y][1],getMatVal(nn.outputLayer,0,0),training_outputs[y][0]);
			 double *deltaO = deltaOut(nn,y);
			double *deltaH = deltaHiddens(nn,deltaO);
			applyChanges(nn,deltaO,deltaH,lr,y);
		}
		nb++;
	}
}
