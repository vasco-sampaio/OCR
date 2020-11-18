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

	res.hiddenBias = initRandMatrix(1,hiddens);
	res.outputBias = initRandMatrix(1,outputs);

	res.hiddenWeights = initRandMatrix(inputs,hiddens); //2*2 matrix
	res.outputWeights = initRandMatrix(hiddens,outputs);//2*1 matrix


	return res;
}



// FORWARD PROP

// Compute the hidden layer activation
void actvHidden(neuralNetwork nn, double *inputs)
{
	for(int i = 0; i < nn.numHiddens; i++)
	{
		double res = getMVal(nn.hiddenBias,0,i);
		
		for(int j = 0;j < nn.numInputs;j++)
		{	
			res += inputs[j] * getMVal(nn.hiddenWeights,j,i);
		}
		setMVal(nn.hiddenLayer,0,i,sigmoid(res));
	}
}



// Compute the output layer activation
void actvOutput(neuralNetwork nn)
{
	for(int i = 0; i < nn.numOutputs; i++)
	{
		double res = getMVal(nn.outputBias,0,i);	
		for(int j = 0; j < nn.numHiddens; j++)
		{
			res += getMVal(nn.hiddenLayer,0,j) * getMVal(nn.outputWeights,j,i);
		}
		setMVal(nn.outputLayer,0,i,sigmoid(res));
	}
}



//Give the output of the neural network
char Output(neuralNetwork nn,char *aNum)
{
	char res;	
	int nbOut = nn.numOutputs;
	double max = getMVal(nn.outputLayer,0,0);
	int tmp = 0;
	for(int i = 0; i < nbOut;i++)
	{
		if(getMVal(nn.outputLayer,0,i) > max)
		{
			tmp = i;
			max = getMVal(nn.outputLayer,0,i);
		}
	}
	res = aNum[tmp];	
	return res;
}
