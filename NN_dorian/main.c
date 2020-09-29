# include "NeuralNetwork.h"
# include "Utility.h"
# include "Backprop.h"

# include <stdlib.h>
# include <stdio.h>
# include <time.h>

static const int trainingSetSize = 4;
double training_inputs[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
double training_outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};
double *inputs = training_inputs;
double *outputs = training_outputs;

int main()
{
	srand(time(NULL));
	initNeuralNetwork(2, 2, 1);

	train(trainingSetSize, inputs, outputs);

	computeValue((inputs));
	computeValue((inputs + 2));
	computeValue((inputs + 4));
	computeValue((inputs + 6));
	printf("%f", *outputLayerNodes);
}
