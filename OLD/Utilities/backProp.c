# include "utility.h"
# include "neuralNet.h"



// Calculate the error of the network and adjust the weights and biases
// expOutputs : outputs the network should have sent back
void backProp(neunet_t *nn, matrix_t inputs, matrix_t expOutputs, double learningRate)
{
	double applyLR(double x){return x * learningRate;}
	double mult(double x, double y){return x*y;}

	// I - CALCULATE THE ERROR

	// Calculate the error of output layer
	matrix_t errorOut = matSub(expOutputs, nn->outputAct);
	matrix_t tmp1 = matFunc(nn->outputAct, dSigmoid);

	matrix_t dOut = dualMatFunc(tmp1, errorOut, mult);


	// Calculate the error of the hidden layer
	matrix_t tmp2 = matTrans(nn->outputWeights);
	matrix_t errorH = matMul(dOut, tmp2);
	matrix_t tmp3 = matFunc(nn->hiddenAct, dSigmoid);
	matrix_t dHidden = dualMatFunc(errorH, tmp3, mult);


	// II - ADJUST WEIGHTS AND BIASES


	matrix_t tmp4 = matTrans(nn->hiddenAct);
	matrix_t tmp5 = matMul(tmp4, dOut);
	matrix_t tmp6 = matFunc(tmp5, applyLR);
	matrix_t tmp7 = matAdd(nn->outputWeights, tmp6);
	nn->outputWeights = tmp7;

	
	matrix_t tmp8 = matTrans(dOut);
	matrix_t tmp9 = matFunc(tmp8, applyLR);
	matrix_t tmp10 = matAdd(tmp9, nn->outputBiases);
	nn->outputBiases = tmp10;


	matrix_t tmp11 = matTrans(inputs);
	matrix_t tmp12 = matMul(tmp11, dHidden);
	matrix_t tmp13 = matFunc(tmp12, applyLR);
	matrix_t tmp14 = matAdd(tmp13, nn->hiddenWeights);
	nn->hiddenWeights = tmp14;


	matrix_t tmp16 = matFunc(dHidden, applyLR);
	matrix_t tmp17 = matAdd(tmp16, nn->hiddenBiases);
	nn->hiddenBiases = tmp17;


	freeMatrix(tmp1);
	freeMatrix(tmp2);
	freeMatrix(tmp3);
	freeMatrix(tmp4);
	freeMatrix(tmp5);
	freeMatrix(tmp6);
	freeMatrix(tmp8);
	freeMatrix(tmp9);
	freeMatrix(tmp11);
	freeMatrix(tmp12);
	freeMatrix(tmp13);
	freeMatrix(tmp16);





}


