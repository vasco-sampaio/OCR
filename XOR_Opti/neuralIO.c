# include "nn.h"
# include "matrix.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>
# include <json.h>


// Define hierarchy tags of the JSON file
# define JSON_NB_INPUTS			"nbInputs"
# define JSON_NB_HIDDENS		"nbHiddens"
# define JSON_NB_OUTPUTS		"nbOutputs"
# define JSON_HIDDEN_BIASES		"hBiases"
# define JSON_OUTPUT_BIASES		"oBiases"
# define JSON_HIDDEN_WEIGHTS	"hWeights"
# define JSON_OUTPUT_WEIGHTS	"oWeights"



// We are using the json-c libray to effectively save 






neuralNetwork fileToNeuralNet(char *path)
{
	json_object *neuralNet_json;

	// I - GET THE JSON OBJECT FROM THE FILE
	neuralNet_json = json_object_from_file(path);

	
	// II - GET THE SIZE OF THE LAYERS
	int nbInputs, nbHiddens, nbOutputs;
	json_object *js_Inputs, *js_Hiddens, *js_Outputs;
	json_object_object_get_ex(neuralNet_json, JSON_NB_INPUTS, &js_Inputs);
	json_object_object_get_ex(neuralNet_json, JSON_NB_HIDDENS, &js_Hiddens);
	json_object_object_get_ex(neuralNet_json, JSON_NB_OUTPUTS, &js_Outputs);
	
	nbInputs = json_object_get_int(js_Inputs);
	nbHiddens = json_object_get_int(js_Hiddens);
	nbOutputs = json_object_get_int(js_Outputs);

	
	// III - INITIALIZE THE NETWORK
	neuralNetwork RES = initNN(nbInputs, nbHiddens, nbOutputs);

	
	// IV - GET THE BIASES

	// Generic json_object pointer, avoid multiple definition
	json_object *bias;
	double val_bias;
	
	// IV.a - Hidden Biases
	json_object *hBiases;
	json_object_object_get_ex(neuralNet_json, JSON_HIDDEN_BIASES, &hBiases);
	for(int i = 0; i < RES.numHiddens; ++i)
	{
		bias = json_object_array_get_idx(hBiases, i);
		val_bias = json_object_get_double(bias);
		setMatVal(RES.hiddenLayerBias, 0, i, val_bias);
	}

	// IV.b - Output Biases
	json_object *oBiases;
	json_object_object_get_ex(neuralNet_json, JSON_OUTPUT_BIASES, &oBiases);
	for(int i = 0; i < RES.numOutputs; ++i)
	{
		bias = json_object_array_get_idx(oBiases, i);
		val_bias = json_object_get_double(bias);
		setMatVal(RES.outputLayerBias, 0, i, val_bias);
	}


	// V - GET THE WEIGHTS
	

	// Generic json_object pointer and double value to avoid multiple definition
	json_object *weight_list;
	json_object *weight;
	double val_weight;

	// V.a - Hidden Weights
	json_object *hWeights;
	json_object_object_get_ex(neuralNet_json, JSON_HIDDEN_WEIGHTS, &hWeights);
	for(int i = 0; i < RES.numInputs; ++i)
	{
		weight_list = json_object_array_get_idx(hWeights, i);
		for(int j = 0; j < RES.numHiddens; ++j)
		{
			weight = json_object_array_get_idx(weight_list, j);
			val_weight = json_object_get_double(weight);
			setMatVal(RES.hiddenWeights, i, j, val_weight);
		}
	}

	// V.b - Output Weights
	json_object *oWeights;
	json_object_object_get_ex(neuralNet_json, JSON_OUTPUT_WEIGHTS, &oWeights);
	for(int i = 0; i < RES.numHiddens; ++i)
	{
		weight_list = json_object_array_get_idx(oWeights, i);
		for(int j = 0; j < RES.numOutputs; ++j)
		{
			weight = json_object_array_get_idx(weight_list, j);
			val_weight = json_object_get_double(weight);
			setMatVal(RES.outputWeights, i, j, val_weight);
		}
	}



	// VI - RETURN
	return RES;
}


void neuralNetToFile(neuralNetwork nn, char *path)
{
	// First create the Neural Network object
	json_object *js_nn = json_object_new_object();
	
	// Then the layer sizes objects
	json_object *nbInputs = json_object_new_int(nn.numInputs);
	json_object *nbHiddens = json_object_new_int(nn.numHiddens);
	json_object *nbOutputs = json_object_new_int(nn.numOutputs);

	// Biases objects
	json_object *hBiases = json_object_new_array();
	json_object *oBiases = json_object_new_array();

	// Hidden Biases
	for(int i = 0; i < nn.numHiddens; ++i)
	{
		json_object *bias = json_object_new_double(getMatVal(nn.hiddenLayerBias, 0, i));
		json_object_array_add(hBiases, bias);
	}

	// Output Biases
	for(int i = 0; i < nn.numOutputs; ++i)
	{
		json_object *bias = json_object_new_double(getMatVal(nn.outputLayerBias, 0, i));
		json_object_array_add(oBiases, bias);
	}
	

	// Weight objects
	json_object *hWeights = json_object_new_array();
	json_object *oWeights = json_object_new_array();

	// Hidden Weights
	for(int i = 0; i < nn.numInputs; ++i)
	{
		json_object *input_to_hiddens = json_object_new_array();
		for(int j = 0; j < nn.numHiddens; ++j)
		{
			json_object *i_to_h = json_object_new_double(getMatVal(nn.hiddenWeights, i, j));
			json_object_array_add(input_to_hiddens, i_to_h);
		}
		json_object_array_add(hWeights, input_to_hiddens);
	}
	

	// Output Weights
	for(int i = 0; i < nn.numHiddens; ++i)
	{
		json_object *hidden_to_outputs = json_object_new_array();
		for(int j = 0; j < nn.numOutputs; ++j)
		{
			json_object *h_to_o = json_object_new_double(getMatVal(nn.outputWeights, i, j));
			json_object_array_add(hidden_to_outputs, h_to_o);
		}
		json_object_array_add(oWeights, hidden_to_outputs);
	}



	// Link them
	json_object_object_add(js_nn, JSON_NB_INPUTS, nbInputs);
	json_object_object_add(js_nn, JSON_NB_HIDDENS, nbHiddens);
	json_object_object_add(js_nn, JSON_NB_OUTPUTS, nbOutputs);
	json_object_object_add(js_nn, JSON_HIDDEN_BIASES, hBiases);
	json_object_object_add(js_nn, JSON_OUTPUT_BIASES,  oBiases);
	json_object_object_add(js_nn, JSON_HIDDEN_WEIGHTS, hWeights);
	json_object_object_add(js_nn, JSON_OUTPUT_WEIGHTS, oWeights);





	// Write the json Object to the file
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
