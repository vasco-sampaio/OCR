# include "utility.h"
# include "neuralNet.h"

# include <stdlib.h>
# include <err.h>



neunet_t initNeuralNet(int inputSize, int hiddenSize, int outputSize)
{
	neunet_t nn;

	nn.nbInputs = inputSize;
	nn.nbHiddens = hiddenSize;
	nn.nbOutputs = outputSize;

	nn.hiddenAct = initMatrix(1, hiddenSize);
	nn.outputAct = initMatrix(1, outputSize);

	nn.hiddenBiases = initRandMatrix(1, hiddenSize);
	nn.outputBiases = initRandMatrix(1, outputSize);

	nn.hiddenWeights = initRandMatrix(inputSize, hiddenSize);
	nn.outputWeights = initRandMatrix(hiddenSize, outputSize);

	return nn;
}

