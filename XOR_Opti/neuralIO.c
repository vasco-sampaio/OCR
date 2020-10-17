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
		setMVal(RES.hiddenLayerBias, 0, i, val_bias);
	}

	// IV.b - Output Biases
	json_object *oBiases;
	json_object_object_get_ex(neuralNet_json, JSON_OUTPUT_BIASES, &oBiases);
	for(int i = 0; i < RES.numOutputs; ++i)
	{
		bias = json_object_array_get_idx(oBiases, i);
		val_bias = json_object_get_double(bias);
		setMVal(RES.outputLayerBias, 0, i, val_bias);
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
			setMVal(RES.hiddenWeights, i, j, val_weight);
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
			setMVal(RES.outputWeights, i, j, val_weight);
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
		json_object *bias = json_object_new_double(getMVal(nn.hiddenLayerBias, 0, i));
		json_object_array_add(hBiases, bias);
	}

	// Output Biases
	for(int i = 0; i < nn.numOutputs; ++i)
	{
		json_object *bias = json_object_new_double(getMVal(nn.outputLayerBias, 0, i));
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
			json_object *i_to_h = json_object_new_double(getMVal(nn.hiddenWeights, i, j));
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
			json_object *h_to_o = json_object_new_double(getMVal(nn.outputWeights, i, j));
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

