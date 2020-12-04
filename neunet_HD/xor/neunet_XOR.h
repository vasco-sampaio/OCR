# ifndef NEUNET_H
# define NEUNET_H

/*
# define IMG_SIDE 30
# define INPUTS IMG_SIDE*IMG_SIDE
# define HIDDEN 22
# define OUTPUTS 62
*/
# define INPUTS 2
# define HIDDEN 2
# define OUTPUTS 1

typedef struct nn
{
	int nb_inputs;
	int nb_hidden;
	int nb_outputs;

	// Inputs array
	double inputs[INPUTS];
	double exp_outputs[OUTPUTS];

	// Biases arrays
	double biases_h[HIDDEN];
	double biases_o[OUTPUTS];

	// Weights arrays
	double weights_i_h[INPUTS][HIDDEN];
	double weights_h_o[HIDDEN][OUTPUTS];

	// Activations arrays
	double act_h[HIDDEN];
	double act_o[OUTPUTS];
	
	// Delta Weights arrays
	double d_weights_i_h[INPUTS][HIDDEN];
	double d_weights_h_o[HIDDEN][OUTPUTS];

	// Delta Output layer
	double d_output[OUTPUTS];

	// Delta Hidden layer
	double d_hidden[HIDDEN];


} neunet_t;

neunet_t *init_neunet();

void neunet_train(neunet_t *nn, double *in, double *out, double lr);


# endif
