# ifndef _NEURALNET_H_
# define _NEURALNET_H_

# include "utility.h"



//Structure definition for 
// single hidden layer neural network
typedef struct
neunet{
	int nbInputs;
	int nbHiddens;
	int nbOutputs;

	matrix_t hiddenAct;
	matrix_t outputAct;

	matrix_t hiddenBiases;
	matrix_t outputBiases;
	matrix_t hiddenWeights;
	matrix_t outputWeights;

}neunet_t;

neunet_t initNeuralNet(int inputs, int hiddens, int outputs);

void backProp(neunet_t *nn, matrix_t inputs, matrix_t outputs, double lr);


# endif //_NEURALNET_H_
