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



// ############################### FILE LOADING ###############################


// Get an int in the tegged child of parent
int getSize(json_object *parent, char *tag)
{
	json_object *child;
	json_object_object_get_ex(parent, tag, &child);
	return json_object_get_int(child);

}



// Specially made function to load an array in a bias matrix
void fillBiases(matrix_t *biases, json_object *parent, char *tag)
{
	json_object *biases_json;
	json_object *bias;
	double bias_val;
	json_object_object_get_ex(parent, tag, &biases_json);

	for(int i = 0; i < biases->cols; ++i)
	{
		bias = json_object_array_get_idx(biases_json, i);
		bias_val = json_object_get_double(bias);
		setMVal(*biases, 0, i, bias_val);
	}

}



// Same but for 2D weights matrices
void fillWeights(matrix_t *weights, json_object *parent, char *tag)
{
	json_object *w_rows;
	json_object *w_cols;
	json_object *weight;
	double w_val;
	json_object_object_get_ex(parent, tag, &w_rows);

	for(int i = 0; i < weights->rows; ++i)
	{
		w_cols = json_object_array_get_idx(w_rows, i);
		for(int j = 0; j < weights->cols; ++j)
		{
			weight = json_object_array_get_idx(w_cols, j);
			w_val = json_object_get_double(weight);
			setMVal(*weights, i, j, w_val);
		}
	}
}



// Main loading function, load a neural net from a json file
neuralNetwork fileToNeuralNet(char *path)
{
	// I - GET THE JSON OBJECT FROM THE FILE
	json_object *neunet_json;
	neunet_json = json_object_from_file(path);
	
	// II - GET THE SIZE OF THE LAYERS
	int nbInputs = getSize(neunet_json, JSON_NB_INPUTS);
	int nbHiddens = getSize(neunet_json, JSON_NB_HIDDENS);
	int nbOutputs = getSize(neunet_json, JSON_NB_OUTPUTS);
	
	// III - INITIALIZE THE NETWORK
	neuralNetwork RES = initNN(nbInputs, nbHiddens, nbOutputs);

	// IV - GET THE BIASES
	fillBiases(&RES.hiddenBias, neunet_json, JSON_HIDDEN_BIASES);
	fillBiases(&RES.outputBias, neunet_json, JSON_OUTPUT_BIASES);

	// V - GET THE WEIGHTS
	fillWeights(&RES.hiddenWeights, neunet_json, JSON_HIDDEN_WEIGHTS);
	fillWeights(&RES.outputWeights, neunet_json, JSON_OUTPUT_WEIGHTS);
	
	// VI - RETURN
	return RES;
}

// ############################################################################



// ########################### FILE WRITING ###################################

// Return a 1D JSON array from the given 1D matrix
json_object *biases_object(matrix_t biases)
{
	json_object *biases_json = json_object_new_array();
	json_object *bias;
	double val;

	for(int i = 0; i < biases.cols; ++i)
	{
		val = getMVal(biases, 0, i);
		bias = json_object_new_double(val);
		json_object_array_add(biases_json, bias);
	}
	return biases_json;
}



//
json_object *weights_object(matrix_t weights)
{
	json_object *weights_json = json_object_new_array();
	json_object *w_list;
	json_object *weight;
	double val;

	for(int i = 0; i < weights.rows; ++i)
	{
		w_list = json_object_new_array();
		for(int j = 0; j < weights.cols; ++j)
		{
			val = getMVal(weights, i, j);
			weight = json_object_new_double(val);
			json_object_array_add(w_list, weight);
		}
		json_object_array_add(weights_json, w_list);
	}
	return weights_json;
}



// Main writing function
void neuralNetToFile(neuralNetwork nn, char *path)
{
	// First create the Neural Network object
	json_object *js_nn = json_object_new_object();
	
	// Then the layer sizes objects
	json_object *nbInputs = json_object_new_int(nn.numInputs);
	json_object *nbHiddens = json_object_new_int(nn.numHiddens);
	json_object *nbOutputs = json_object_new_int(nn.numOutputs);

	// Biases
	json_object *hBiases = biases_object(nn.hiddenBias);
	json_object *oBiases = biases_object(nn.outputBias);

	// Weights
	json_object *hWeights = weights_object(nn.hiddenWeights);
	json_object *oWeights = weights_object(nn.outputWeights);


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

