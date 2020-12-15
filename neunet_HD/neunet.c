# include "../types.h"

# include <stdlib.h>
# include <stdio.h>

// Set a default alpha which is often 0.9
# define DEFAULT_ALPHA 0.9


// Neural net output disposal [1-9]-[a-z]-[A-Z]
char nn_ocr_out[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPKRSTUVWXYZ";

// I know global variables aren't good but 
// I really don't know where to put it otherwise


neunet_t *init_neunet()
{
	neunet_t *res = calloc(1, sizeof(neunet_t));

	res->highest_output = 0;

	// I_H Weights init
	for(int i = 0; i < NN_INPUTS; ++i)
		for(int j = 0; j < NN_HIDDENS; ++j)
			res->weights_i_h[i][j] = xavier_init(NN_INPUTS);

	// H_O Weights init
	for(int i = 0; i < NN_HIDDENS; ++i)
		for(int j = 0; j < NN_OUTPUTS; ++j)
			res->weights_h_o[i][j] = xavier_init(NN_HIDDENS);

	return res;
}


// Compute the neural net output
void forward_prop(neunet_t *nn)
{
	double sum;	

	// calculate the hidden activation
	for(int i = 0; i < NN_HIDDENS; ++i)
	{
		sum = nn->biases_h[i];
		for(int j = 0; j < NN_INPUTS; ++j)
			sum += nn->weights_i_h[j][i] * nn->inputs[j];
		nn->act_h[i] = sigmoid(sum);
	}

	// Calculate the output layer activation
	for(int i = 0; i < NN_OUTPUTS; ++i)
	{
		sum = nn->biases_o[i];
		for(int j = 0; j < NN_HIDDENS; ++j)
			sum += nn->weights_h_o[j][i] * nn->act_h[j];
		nn->act_o[i] = sigmoid(sum);

		// update the *highest_output* if necessary
		if(nn->act_o[nn->highest_output] < nn->act_o[i])
			nn->highest_output = i;
	}
}


void backward_prop(neunet_t *nn)
{
	double error;	

	// Calculate the delta of output layer
	for(int i = 0; i < NN_OUTPUTS; ++i)
	{
		error = dSigmoid(nn->act_o[i]);
		error *= (nn->exp_outputs[i] - nn->act_o[i]);
		nn->d_output[i] = error;
	}

	double x;
	// Calculate the delta of hidden layer
	for(int i = 0; i < NN_HIDDENS; ++i)
	{
		x = 0;
		for(int j = 0; j < NN_OUTPUTS; ++j)
			x += nn->d_output[j] * nn->weights_h_o[i][j];
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
	for(int i = 0; i < NN_HIDDENS; ++i)
	{
		for(int j = 0; j < NN_INPUTS; ++j)
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
	for(int i = 0; i < NN_OUTPUTS; ++i)
	{
		for(int j = 0; j < NN_HIDDENS; ++j)
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
	for(int i = 0; i < NN_HIDDENS; ++i)
	{
		err = nn->d_hidden[i];
		err *= lr;

		nn->biases_h[i] += err;
	}

	// Update output biases
	for(int i = 0; i < NN_OUTPUTS; ++i)
	{
		err = nn->d_output[i];
		err *= lr;

		nn->biases_o[i] += err;
	}
}


void neunet_train(neunet_t *nn, double *inputs, double *Xout, double lr)
{
	// Fill the inputs and outputs of the network
	for(int i = 0; i < NN_INPUTS; ++i)
		nn->inputs[i] = inputs[i];
	for(int i = 0; i < NN_OUTPUTS; ++i)
		nn->exp_outputs[i] = Xout[i];


	forward_prop(nn);
	backward_prop(nn);
	update_weights(nn, lr, DEFAULT_ALPHA);
	update_biases(nn, lr);
}




// compute the output of the network with the given inputs,
// and return the corresponding char
char neural_net_ask(neunet_t *nn, double *inputs)
{
	// fill the inputs of the network
	for(int i = 0; i < NN_INPUTS; ++i)
		nn->inputs[i] = inputs[i];

	// Compute the output
	forward_prop(nn);

	// Return the wanted char
	return nn_ocr_out[nn->highest_output];
}

// Return the index of the max value in an example output of the neunet
char expected_output(size_t size,double *expOut)
{
	double tmp = *(expOut);
	size_t res = 0;
	for(size_t i = 0; i < size;i++)
	{
		if(*(expOut+i) > tmp)
		{
			tmp = *(expOut+i);
			res = i;
		}
	}
	return nn_ocr_out[res];
}


//determine the squqred_error of the neunet
double squared_error(neunet_t *nn)
{
    double s_error = 0;
    double tmp = 0;
    for(int i = 0; i < NN_OUTPUTS;i++)
    {
        tmp = nn->exp_outputs[i] - nn->act_o[i];
        s_error += tmp*tmp;
    }
    return s_error/2;
}
