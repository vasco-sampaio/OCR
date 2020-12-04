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

	res->gen_error = 0;
	res->global_error = 0;
	res->max_glb_err = 0;


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


// Calculate the squared error
double neunet_get_error(neunet_t *nn)
{
	double e = 0;
	nn->gen_error = 0;

	for(int out = 0; out < OUTPUTS; ++out)
	{
		e = (nn->act_o[out] - nn-> exp_outputs[out]);
		e *= e;

		nn->gen_error += e;
	}

	return nn->gen_error;
}

char Output(neuralNetwork nn,char *aNum)
{
	char res;
        int nbOut = nn.numOutputs;
        double max = getMVal(nn.outputLayer,0,0);
        int tmp = 0;
        for(int i = 0; i < nbOut;i++)
	 {
                  if(getMVal(nn.outputLayer,0,i) > max)
                  {
                          tmp = i;
                          max = getMVal(nn.outputLayer,0,i);
                  }
          }
          res = *(aNum+tmp);
          return res;
}
 
char ExpOut(size_t size,double *expOut,char *aNum)
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
          return *(aNum+res);
  }

