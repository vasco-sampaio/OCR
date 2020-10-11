# include "neuralNet.h"
# include "forwardProp.h"
# include "utility.h"
# include "train.h"


static const int trainingSetSize = 4;
double training_inputs[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
double training_outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};
double *inputs = training_inputs;
double *outputs = training_outputs;


int main()
{
	neunet_t XOR = initNeuralNet(2,2,1);
/*	
	setMatVal(XOR.hiddenWeights,0,0,3.7);
	setMatVal(XOR.hiddenWeights,0,1,5.9);
	setMatVal(XOR.hiddenWeights,1,0,3.7);
	setMatVal(XOR.hiddenWeights,1,1,5.9);
	setMatVal(XOR.outputWeights,0,0, -8.1);
	setMatVal(XOR.outputWeights,1,0, 7.5);

	setMatVal(XOR.hiddenBiases, 0, 0, -5.6);
	setMatVal(XOR.hiddenBiases, 0, 1, -2.4);
	setMatVal(XOR.outputBiases, 0, 0, -3.6);

	matrix_t inputs = initMatrix(1,2);
	setMatVal(inputs, 0, 0, 1.0);
	setMatVal(inputs, 0, 1, 0.0);

	printMatrix(inputs);

	XOR.outputAct = forwardProp(XOR, inputs);
	printMatrix(XOR.outputAct);

	*/
	trainNetwork(&XOR, inputs, outputs, 0.1, trainingSetSize, 10000);	

}
