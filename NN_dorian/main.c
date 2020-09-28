# include "NeuralNetwork.h"
# include "Utility.h"
# include <stdlib.h>
# include <stdio.h>

static const int trainingSetSize = 4;
double training_inputs[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
double training_outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};
double *inputs = training_inputs;
double *outputs = training_outputs;

int main()
{
	initNeuralNetwork(2, 2, 1);

	int *test = shuffledList(10);
	for(int i = 0; i < 10; i++)
	{
		printf("%i ", *(test + i));
	}
}
