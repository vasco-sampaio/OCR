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
		setMVal(nn.outputLayerBias, 0, i, getMVal(nn.outputLayerBias, 0, i) + (dO[i] * lr));
		for(int j = 0; j < nn.numHiddens; j++)
		{	
			double add = getMVal(nn.outputWeights, j ,i);
			setMVal(nn.outputWeights, j, i, add + getMVal(nn.hiddenLayer, 0, j) * dO[i] * lr);
		}
	}
	
	
	//Changes on the hidden weights and bias
	for(int i = 0; i < nn.numHiddens; i++)
	{
		setMVal(nn.hiddenLayerBias, 0, i, getMVal(nn.hiddenLayerBias, 0, i) + dH[i] * lr);
		for(int j = 0; j < nn.numInputs; j++)
		{
			double add2 = getMVal(nn.hiddenWeights, j, i);
			setMVal(nn.hiddenWeights, j, i, add2 + inputs[j] * dH[i] * lr);
		}
	}
}



// Train the network with the given data over several epochs
void train(neuralNetwork nn, double *inputs, double *expOutputs, int setSize, int epochs, double lr)
{
	int nb = 0;

	// Iterate through the number of generations to compute
	while(nb < epochs)
	{
		int *order = shuffledList(setSize);
		
		// Iterate through the whole set of inputs, in a random order
		for(int x = 0; x<4;x++)
		{
			int index = order[x];
			 
			double *curInputs = inputs + index * nn.numInputs;
			double *curOutputs = expOutputs + index * nn.numOutputs;

			// Feed forward the network
			actvHidden(nn, curInputs);
			actvOutput(nn);

			// Some printing YUP
		//	printf("Test with: %f and %f got %f but expect %f \n", curInputs[0], curInputs[1], getMVal(nn.outputLayer,0,0),curOutputs[0]);

			// Calculate the error of the actual input
			double *deltaO = deltaOut(nn, curOutputs);
			double *deltaH = deltaHiddens(nn, deltaO);

			// Use data to update the network
			applyChanges(nn, deltaO, deltaH, lr, curInputs);
		}
		nb++;
	}
}



// XOR test
void test(neuralNetwork nn,double *inputs,double *output)
{
	printf("XOR neural network: \n");
	for(int x = 0;x < 4;x++)
	{
		double *curInputs = inputs + x * nn.numInputs;
		double *curOutput = output + x * nn.numOutputs;
		actvHidden(nn, curInputs);
		actvOutput(nn);
		printf("Inputs: %lf and %lf got %lf expected %f \n", curInputs[0],curInputs[1],getMVal(nn.outputLayer,0,0),curOutput[x]);
	}
}
