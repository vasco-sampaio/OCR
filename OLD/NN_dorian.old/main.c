# include "neuralNetwork.h"

# include <stdio.h>

static const int trainingSetSize = 4;
double training_inputs[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
double training_outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};
double *inputs = training_inputs;
double *outputs = training_outputs;

int main()
{
	initNeuralNetwork(2,2,1);

	printNetwork();
	
	trainNetwork(trainingSetSize, training_inputs, training_outputs);

	printNetwork();

	computeNetwork((inputs));
	printf("Testing with:\n%f\n%f\ngot : %f\n",*(inputs), *(inputs + 1), *outputAct);
	computeNetwork((inputs + 2));
	printf("Testing with:\n%f\n%f\ngot : %f\n",*(inputs + 2), *(inputs + 3), *outputAct);
	computeNetwork((inputs + 4));
	printf("Testing with:\n%f\n%f\ngot : %f\n",*(inputs + 4), *(inputs + 5), *outputAct);
	computeNetwork((inputs + 6));
	printf("Testing with:\n%f\n%f\ngot : %f\n",*(inputs + 6), *(inputs + 7), *outputAct);
}
