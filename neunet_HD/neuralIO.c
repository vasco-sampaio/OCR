# include "neunet.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <err.h>
# include <json.h>


// Define hierarchy tags of the JSON file
# define JSON_NB_INPUTS			"nb_inputs"
# define JSON_NB_HIDDENS		"nb_hiddens"
# define JSON_NB_OUTPUTS		"nb_outputs"
# define JSON_HIDDEN_BIASES		"h_biases"
# define JSON_OUTPUT_BIASES		"o_biases"
# define JSON_HIDDEN_WEIGHTS	"h_weights"
# define JSON_OUTPUT_WEIGHTS	"o_weights"


// We can't use functions because parameters are static arrays
# define fill_weights(w_dst, js_w_src, w_in, w_out)					\
	for(int i = 0; i < w_in; ++i){									\
		json_object *col = json_object_array_get_idx(js_w_src, i);	\
		for(int j = 0; j < w_out; ++j){								\
			json_object *w_j = json_object_array_get_idx(col, j);	\
			double val = json_object_get_double(w_j);				\
			w_dst[i][j] = val;										\
		}															\
	}

// We must use a macro and not a function because
// weight arrays are static and thus can't be function parameters
# define weights_object(jobj, w, w_in, w_out)					\
	for(int i = 0; i < w_in; ++i)								\
{																\
	json_object *w_list = json_object_new_array();				\
	for(int j = 0; j < w_out; ++j)								\
	{															\
		json_object *weight = json_object_new_double(w[i][j]);	\
		json_object_array_add(w_list, weight);					\
	}															\
	json_object_array_add(jobj, w_list);						\
}													




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
void fillBiases(double *biases, int w_l, json_object *parent, char *tag)
{
	json_object *biases_json;
	json_object *bias;
	double bias_val;
	json_object_object_get_ex(parent, tag, &biases_json);

	for(int i = 0; i < w_l; ++i)
	{
		bias = json_object_array_get_idx(biases_json, i);
		bias_val = json_object_get_double(bias);
		biases[i] = bias_val;
	}

}




// Main loading function, load a neural net from a json file
neunet_t *fileToNeuralNet(char *path)
{
	const char *json_error;

	// I - GET THE JSON OBJECT FROM THE FILE
	json_object *neunet_json;
	neunet_json = json_object_from_file(path);
	json_error = json_util_get_last_err();

	if(json_error)
		errx(1, json_error);



	// II - GET THE SIZE OF THE LAYERS
	int nbInputs = getSize(neunet_json, JSON_NB_INPUTS);
	int nbHiddens = getSize(neunet_json, JSON_NB_HIDDENS);
	int nbOutputs = getSize(neunet_json, JSON_NB_OUTPUTS);

	if(nbInputs != NN_INPUTS || nbHiddens != NN_HIDDENS || nbOutputs != NN_OUTPUTS)
	{
		errx(1,	"Error: loading neural_net: wrong format");
	}

	// III - INITIALIZE THE NETWORK
	neunet_t *RES = init_neunet();

	// IV - GET THE BIASES
	fillBiases(RES->biases_h, NN_HIDDENS, neunet_json, JSON_HIDDEN_BIASES);
	fillBiases(RES->biases_o, NN_OUTPUTS, neunet_json, JSON_OUTPUT_BIASES);

	// V - GET THE WEIGHTS
	json_object *weights_buf;

	// Hidden weights
	json_object_object_get_ex(neunet_json, JSON_HIDDEN_WEIGHTS, &weights_buf);
	fill_weights(RES->weights_i_h, weights_buf, NN_INPUTS, NN_HIDDENS);

	// Output weights
	json_object_object_get_ex(neunet_json, JSON_OUTPUT_WEIGHTS, &weights_buf);
	fill_weights(RES->weights_h_o, weights_buf, NN_HIDDENS, NN_OUTPUTS);
	

	// VI - RETURN
	return RES;
}

// ############################################################################



// ########################### FILE WRITING ###################################


// Return a 1D JSON array from the given 1D matrix
json_object *biases_object(double *biases, int arr_len)
{
	json_object *biases_json = json_object_new_array();
	json_object *bias;

	for(int i = 0; i < arr_len; ++i)
	{
		bias = json_object_new_double(biases[i]);
		json_object_array_add(biases_json, bias);
	}
	return biases_json;
}




// Main writing function
void neuralNetToFile(neunet_t *nn, char *path)
{
	// First create the Neural Network object
	json_object *js_nn = json_object_new_object();

	// Then the layer sizes objects
	json_object *nbInputs = json_object_new_int(NN_INPUTS);
	json_object *nbHiddens = json_object_new_int(NN_HIDDENS);
	json_object *nbOutputs = json_object_new_int(NN_OUTPUTS);

	// Biases
	json_object *hBiases = biases_object(nn->biases_h, NN_HIDDENS);
	json_object *oBiases = biases_object(nn->biases_o, NN_OUTPUTS);

	// Weights
	json_object *hWeights = json_object_new_array();
	weights_object(hWeights, nn->weights_i_h, NN_INPUTS, NN_HIDDENS);

	json_object *oWeights = json_object_new_array();
	weights_object(oWeights, nn->weights_h_o, NN_HIDDENS, NN_OUTPUTS);


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

