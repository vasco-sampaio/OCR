# include "train.h"
# include "utility.h"
# include <stdio.h>




//Calculate the error of the output
double* deltaOut(neuralNetwork nn, double *expOutput)
{
	double *deltaOutput = calloc(nn.numInputs, sizeof(double));

	// Iterate through the outputs
	for(int i = 0; i < nn.numOutputs; i++)
	{
		// Get the error by using substraction
		double error = (expOutput[i] - getMVal(nn.outputLayer, 0, i));
		deltaOutput[i] = error * dSigmoid(getMVal(nn.outputLayer, 0, i));
	}
	return deltaOutput;
}



//Calculate the error of the hiddens
double* deltaHiddens(neuralNetwork nn, double* deltaO)
{
	double *deltaHidden = calloc(nn.numHiddens,sizeof(double));

	// Iterate through the hidden layer
	for(int i = 0; i < nn.numHiddens; i++)
	{
		double error = 0;

		// Iterate through the next layer
		for(int j = 0; j < nn.numOutputs; j++)
		{
			// The error of the hidden node is the weighted sum 
			// of the error of the next layer
			error += deltaO[j] * getMVal(nn.outputWeights, i, j);
		}
		deltaHidden[i] = error * dSigmoid(getMVal(nn.hiddenLayer, 0, i));
	}
	return deltaHidden;
}



// Update the weights and biases
void applyChanges(neuralNetwork nn, double *dO, double *dH, double lr, double *inputs)
{ 
	//Changes on the output weights and bias
	for(int i = 0; i < nn.numOutputs; i++)
	{
		addMVal(nn.outputBias, 0, i, (dO[i] * lr));
		for(int j = 0; j < nn.numHiddens; j++)
		{	
			double add = getMVal(nn.outputWeights, j ,i);
			add += getMVal(nn.hiddenLayer, 0, j) * dO[i] * lr;
			setMVal(nn.outputWeights, j, i, add);
		}
	}
	
	
	//Changes on the hidden weights and bias
	for(int i = 0; i < nn.numHiddens; i++)
	{
		setMVal(nn.hiddenBias, 0, i, getMVal(nn.hiddenBias, 0, i) + dH[i] * lr);
		for(int j = 0; j < nn.numInputs; j++)
		{
			double add2 = getMVal(nn.hiddenWeights, j, i);
			setMVal(nn.hiddenWeights, j, i, add2 + inputs[j] * dH[i] * lr);
		}
	}
}



// Train the network with the given data over several epochs
void train(neuralNetwork nn, double *inputs, double *expOutputs, int setSize, int epochs, double lr, char *aNum)
{
	int nb = 0;

	// Iterate through the number of generations to compute
	while(nb < epochs)
	{
		int *order = shuffledList(setSize);
		
		// Iterate through the whole set of inputs, in a random order
		for(int x = 0; x<setSize;x++)
		{
			int index = order[x];
			 
			double *curInputs = inputs + index * nn.numInputs;
			double *curOutputs = expOutputs + index * nn.numOutputs;

			// Feed forward the network
			actvHidden(nn, curInputs);
			actvOutput(nn);


			// Calculate the error of the actual input
			double *deltaO = deltaOut(nn, curOutputs);
			double *deltaH = deltaHiddens(nn, deltaO);

			// Use data to update the network
			applyChanges(nn, deltaO, deltaH, lr, curInputs);
			printf("Output of the NN: %c\n",Output(nn,aNum));
			free(deltaO);
			free(deltaH);
			//free(curInputs);
			//free(curOutputs);
		}
		nb++;
		free(order);
	}
}

