# include "nn.h"
# include "matrix.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

// This file is meant to define how to write and read neural networks in files
//
// As there is only one hidden layer in the network we implemented,
// we will follow this organization :
//
// line 0 : number of input neurons : ni
// line 1 : number of hidden neurons: nh
// line 2 : number of output neurons: no
// 
// line 3 : values of hidden biases
// line 4 : values of output biases
//
// ni lines of nh values representing the hidden weights
// nh lines of no values representing the output weights


void writeNeuralNet(char *path, neuralNetwork nn)
{
	// First open the the path at the file, erasing it if it was existing
	FILE *file = fopen(path, "w+");
	
	// Write Lines 0 to 2
	fprintf(file, "%i\n%i\n%i\n", nn.numInputs, nn.numHiddens, nn.numOutputs);
	

	// Write Line 3
	for(int i = 0; i < nn.numHiddens; ++i)
	{
		fprintf(file, "%f ", getMatVal(nn.hiddenLayerBias, 0, i));
	}

	fprintf(file, "\n");


	// Write Line 4
	for(int i = 0; i < nn.numOutputs; ++i)
	{
		fprintf(file, "%f ", getMatVal(nn.outputLayerBias, 0, i));
	}

	fprintf(file, "\n");

	
	// Write hidden weights
	for(int i = 0; i < nn.numInputs; ++i)
	{
		for(int j = 0; j < nn.numHiddens; ++j)
		{
			fprintf(file, "%f", getMatVal(nn.hiddenWeights, i, j));
			if(j != nn.numHiddens - 1)
				fprintf(file,' ');
		}
		fprintf(file, "\n");
	}


	// Write output weights
	for(int i = 0; i < nn.numHiddens; ++i)
	{
		for(int j = 0; j < nn.numOutputs; ++j)
		{
			fprintf(file, "%f", getMatVal(nn.outputWeights, i, j));
			if(j != nn.numOutputs - 1)
				fprintf(file,' ');
		}
		fprintf(file, "\n");
	}

	fclose(file);
}


// Return char pointer of the next not digit char
char *getNextNotDigit(char *str)
{
	char *res = str;
	while(res != ' ' && res != '\n' && res != 0)
	{
		++res;
	}
	
	return res;
}



neuralNetwork readNeuralNetwork(char *str, size_t len)
{
	char *current = str;
	char *next = getNextNotDigit(current);
	
	int nbInputs = strtoul(current, next, 10);
	current = ++next;
	next = getNextNotDigit(current);

	int nbHiddens = strtoul(current, next, 10);
	current = ++next;
	next = getNextNotDigit(current);

	int nbOutputs = strtoul(current, next, 10);


	return initNN(nbInputs, nbHiddens, nbOutputs);
}


neuralNetwork loadNeuralNetwork(char *path)
{
	size_t l = strlen(path);
	if(path[l-3] != '.' || path[l-2] != 'n' || path[l-1] != 'n')
	{
		errx(1, "Can't load this image");
	}

	FILE *file = fopen(path, "r");

	// Get file size
	size_t fileLen = ftell(file);

	// Get the string of the file;
	char *contentBuf = malloc(fileLen);
	fgets(contentBuf, fileLen, file);


	
	return readNeuralNetwork(contentBuf, fileLen);


}



