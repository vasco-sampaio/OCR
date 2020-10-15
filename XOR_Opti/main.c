#include "nn.h"
#include "train.h"
#include "neuralIO.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//debug part

double traininginputs[8] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f };
double trainingoutputs[4] = { 0.0f,1.0f,1.0f,0.0f };
double *trIn = traininginputs;
double *trOut = trainingoutputs;

int main()
{
	srand(time(NULL));
	neuralNetwork xor = initNN(2,2,1);
	//train the network on 10k gens
	train(xor, trIn, trOut, 4, 50000, 1);
	//test the network for the 4possibiblite:s
	test(xor,trIn,trOut);


	printMatrix(xor.hiddenLayerBias);
	printMatrix(xor.outputLayerBias);
	printMatrix(xor.hiddenWeights);
	printMatrix(xor.outputWeights);



	neuralNetToFile(xor, "test.json");

	neuralNetwork JSON = fileToNeuralNet("test.json");

	test(JSON, trIn, trOut);

	printMatrix(JSON.hiddenLayerBias);
	printMatrix(JSON.outputLayerBias);
	printMatrix(JSON.hiddenWeights);
	printMatrix(JSON.outputWeights);


	return 0;
}
