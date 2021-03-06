# include "neunet.h"
# include "utils.h"

# include <stdlib.h>
# include <stdio.h>

// Set a default alpha which il often 0.9
# define DEFAULT_ALPHA 0.9



neunet_t *init_neunet()
{
	neunet_t *res = calloc(1, sizeof(neunet_t));
	res->nb_inputs = INPUTS;
	res->nb_hidden = HIDDEN;
	res->nb_outputs = OUTPUTS;


	// I_H Weights init
	for(int i = 0; i < INPUTS; ++i)
		for(int j = 0; j < HIDDEN; ++j)
			res->weights_i_h[i][j] = xavier_init(INPUTS);
	
	// H_O Weights init
	for(int i = 0; i < HIDDEN; ++i)
		for(int j = 0; j < OUTPUTS; ++j)
			res->weights_h_o[i][j] = xavier_init(HIDDEN);
	
	return res;
}


// Compute the neural net output
void forward_prop(neunet_t *nn)
{
	double sum;	

	// calculate the hidden activation
	for(int i = 0; i < HIDDEN; ++i)
	{
		sum = nn->biases_h[i];
		for(int j = 0; j < INPUTS; ++j)
		{
			sum += nn->weights_i_h[j][i] * nn->inputs[j];
		}
		nn->act_h[i] = sigmoid(sum);
	}

	// Calculate the output layer activation
	for(int i = 0; i < OUTPUTS; ++i)
	{
		sum = nn->biases_o[i];
		for(int j = 0; j < HIDDEN; ++j)
		{
			sum += nn->weights_h_o[j][i] * nn->act_h[j];
		}
		nn->act_o[i] = sigmoid(sum);
	}
}


void backward_prop(neunet_t *nn)
{
	double error;	

	// Calculate the delta of output layer
	for(int i = 0; i < OUTPUTS; ++i)
	{
		error = dSigmoid(nn->act_o[i]);
		error *= (nn->exp_outputs[i] - nn->act_o[i]);
		nn->d_output[i] = error;
	}
		
	double x;
	// Calculate the delta of hidden layer
	for(int i = 0; i < HIDDEN; ++i)
	{
		x = 0;
		for(int j = 0; j < OUTPUTS; ++j)
		{
			x += nn->d_output[j] * nn->weights_h_o[i][j];
		}
		nn->d_hidden[i] = x * dSigmoid(nn->act_h[i]);
	}
}


/* update the weights according to the output error, the learning rate, etc...
 *
 * lr : learning rate
 * alpha : importance of the previous error;
 */
void update_weights(neunet_t *nn, double lr, double alpha)
{
	double w_in;
	double prev_delt;
	double delta;

	// Update weights
		
	// hidden weights
	for(int i = 0; i < HIDDEN; ++i)
	{
		for(int j = 0; j < INPUTS; ++j)
		{
			w_in = nn->inputs[j];
			prev_delt = nn->d_weights_i_h[j][i];
			delta = nn->d_hidden[i];

			nn->weights_i_h[j][i] += lr * delta * w_in + 
													alpha * prev_delt;
			nn->d_weights_i_h[j][i] = lr * delta * w_in;
		}
	}


	// output weights
	for(int i = 0; i < OUTPUTS; ++i)
	{
		for(int j = 0; j < HIDDEN; ++j)
		{
			w_in = nn->act_h[j];
			prev_delt = nn->d_weights_h_o[j][i];
			delta = nn->d_output[i];

			nn->weights_h_o[j][i] += lr * delta * w_in + alpha * prev_delt;
			nn->d_weights_h_o[j][i] = lr * delta * w_in;
		}
	}
}


void update_biases(neunet_t *nn, double lr)
{
	double err;

	// Update hidden biases
	for(int i = 0; i < HIDDEN; ++i)
	{
		err = nn->d_hidden[i];
		err *= lr;

		nn->biases_h[i] += err;
	}

	// Update output biases
	for(int i = 0; i < OUTPUTS; ++i)
	{
		err = nn->d_output[i];
		err *= lr;

		nn->biases_o[i] += err;
	}
}


void neunet_train(neunet_t *nn, double *inputs, double *Xout, double lr)
{
	// Fill the inputs and outputs of the network
	for(int i = 0; i < INPUTS; ++i)
		nn->inputs[i] = inputs[i];
	for(int i = 0; i < OUTPUTS; ++i)
		nn->exp_outputs[i] = Xout[i];


	forward_prop(nn);
	backward_prop(nn);
	update_weights(nn, lr, DEFAULT_ALPHA);
	update_biases(nn, lr);
}
