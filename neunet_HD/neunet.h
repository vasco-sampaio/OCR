# ifndef NEUNET_H
# define NEUNET_H

# include <stdlib.h>
# include "../types.h"

# define IMG_SIDE 30
# define NN_INPUTS IMG_SIDE*IMG_SIDE
# define NN_HIDDENS 22
# define NN_OUTPUTS 62


typedef struct nn
{

	// index out the highest output neuron
	// in the output layer
	int highest_output;

	// Inputs array
	double inputs[NN_INPUTS];
	double exp_outputs[NN_OUTPUTS];

	// Biases arrays
	double biases_h[NN_HIDDENS];
	double biases_o[NN_OUTPUTS];

	// Weights arrays
	double weights_i_h[NN_INPUTS][NN_HIDDENS];
	double weights_h_o[NN_HIDDENS][NN_OUTPUTS];

	// Activations arrays
	double act_h[NN_HIDDENS];
	double act_o[NN_OUTPUTS];
	

	// STORRED ERRORS
	
	// Delta Weights arrays
	double d_weights_i_h[NN_INPUTS][NN_HIDDENS];
	double d_weights_h_o[NN_HIDDENS][NN_OUTPUTS];

	// Delta Output layer
	double d_output[NN_OUTPUTS];

	// Delta Hidden layer
	double d_hidden[NN_HIDDENS];


	double gen_error;
	double global_error;
	double max_glb_err;


} neunet_t;

// Neural net initialisation function
neunet_t *init_neunet();

// Training functions
void neunet_train(neunet_t *nn, double *in, double *out, double lr);
char expected_output(size_t size,double *expOut);

// Get error of the neural net
double neunet_get_error(neunet_t *nn);

// Main neural net function
char neural_net_ask(neunet_t *nn, double *inputs);



# endif
