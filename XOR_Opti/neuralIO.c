# include "nn.h"
# include "matrix.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include <json.h>


// We are using the json-c libray to effectively save 

// TO USE : json_object_to_file()





void fileToNeuralNet(char *path)
{
	json_object *neuralNet_json;

	neuralNet_json = json_object_from_file(path);

	int nbInputs, nbHiddens, nbOutputs;
	//json_object_object_get_ex(neuralNet_json, "nbInputs", &nbInputs);
	//json_object_object_get_ex(neuralNet_json, "nbHiddens", &nbHiddens);
	//json_object_object_get_ex(neuralNet_json, "nbOutputs", &nbOutputs);

	
}


void neuralNetToFile(neuralNetwork nn, char *path)
{
	// First create the Neural Network object
	json_object *js_nn = json_object_new_object();
	
	// Then the layer sizes objects
	json_object *nbInputs = json_object_new_int(nn.numInputs);
	json_object *nbHiddens = json_object_new_int(nn.numHiddens);
	json_object *nbOutputs = json_object_new_int(nn.numOutputs);
	
	// Link them
	json_object_object_add(js_nn, "nbInputs", nbInputs);
	json_object_object_add(js_nn, "nbHiddens", nbHiddens);
	json_object_object_add(js_nn, "nbOutputs", nbOutputs);

	json_object_to_file(path, js_nn);
}





/*
   void writeNeuralNet(char *path, neuralNetwork nn)
   {
// First open the the path at the file, erasing it if it was existing
FILE *file = fopen(path, "w+");

// Write Lines 0 to 2
fprintf(file, "%i\n%i\n%i\n", nn.numInputs, nn.numHiddens, nn.numOutputs);


// Write Line 3
for(int i = 0; i < nn.numHiddens; ++i)
{
fprintf(file, "%f", getMatVal(nn.hiddenLayerBias, 0, i));
if(i != nn.numHiddens - 1)
fprintf(file," ");

}

fprintf(file, "\n");


// Write Line 4
for(int i = 0; i < nn.numOutputs; ++i)
{
fprintf(file, "%f", getMatVal(nn.outputLayerBias, 0, i));
if(i != nn.numOutputs - 1)
fprintf(file," ");
}

fprintf(file, "\n");


// Write hidden weights
for(int i = 0; i < nn.numInputs; ++i)
{
for(int j = 0; j < nn.numHiddens; ++j)
{
fprintf(file, "%f", getMatVal(nn.hiddenWeights, i, j));
if(j != nn.numHiddens - 1)
fprintf(file," ");
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
fprintf(file," ");
}
fprintf(file, "\n");
}

fclose(file);
}


// Return char pointer of the next not digit char
char *getNextNotDigit(char *str)
{
char *res = str;
while(*res != ' ' && *res != '\n' && *res != 0)
{
++res;
}

return res;
}



neuralNetwork readNeuralNetwork(char *str)
{
	char *current = str;
	//Layer sizes
	int nbInputs = strtoul(current, NULL, 10);
	current = getNextNotDigit(current) + 1;

	int nbHiddens = strtoul(current, NULL, 10);
	current = getNextNotDigit(current) + 1;

	int nbOutputs = strtoul(current, NULL, 10);
	current = getNextNotDigit(current) + 1;

	neuralNetwork RES = initNN(nbInputs, nbHiddens, nbOutputs);

	double val;

	// Biases
	for(int i = 0; i < nbHiddens; ++i)
	{
		val = strtod(current, NULL);
		setMatVal(RES.hiddenLayerBias, 0, i, val);
		current = getNextNotDigit(current) + 1;
	}

	for(int i = 0; i < nbHiddens; ++i)
	{
		val = strtod(current, NULL);

		setMatVal(RES.hiddenLayerBias, 0, i, val);
		current = getNextNotDigit(current) + 1;
	}

	// Weights
	for(int i = 0; i < nbInputs; ++i)
	{
		for(int j = 0; j < nbHiddens; ++j)
		{
			val = strtod(current, NULL);
			setMatVal(RES.hiddenWeights, i, j, val);
			current = getNextNotDigit(current) + 1;
		}
	}

	for(int i = 0; i < nbHiddens; ++i)
	{
		for(int j = 0; j < nbOutputs; ++j)
		{
			val = strtod(current, NULL);
			setMatVal(RES.outputWeights, i, j, val);
			current = getNextNotDigit(current) + 1;
		}
	}

	return RES;
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
	char *contentBuf = malloc(1000);
	fgets(contentBuf, -1, file);

	printf("TEST\n");
	printf(contentBuf);


	fclose(file);

	return readNeuralNetwork(contentBuf);


}


*/
