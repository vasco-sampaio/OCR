# include "neuralNet.h"
# include "utility.h"


// Calculate the output of the network with given inputs
void forwardProp(neunet_t *nn, matrix_t inputs)
{
	// Compute hidden layer activation
	matrix_t tmp1 = matMul(inputs, nn->hiddenWeights);
	matrix_t tmp2 = matAdd(tmp1, nn->hiddenBiases);
	matrix_t tmp3 = matFunc(tmp2, sigmoid);
	nn->hiddenAct = tmp3;

	// Free temp matrices
	freeMatrix(tmp1);
	freeMatrix(tmp2);
	
	// Compute output layer activation
	matrix_t tmp4 = matMul(tmp3, nn->outputWeights);
	matrix_t tmp5 = matAdd(tmp4, nn->outputBiases);
	matrix_t tmp6 = matFunc(tmp5, sigmoid);
	nn->outputAct = tmp6;

	// Free temp matrices
	freeMatrix(tmp3);
	freeMatrix(tmp4);
	freeMatrix(tmp5);
	freeMatrix(tmp6);

}

