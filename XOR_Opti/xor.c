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


// XOR test
void test(neuralNetwork nn,double *inputs,double *output)
{
	printf("XOR neural network: \n");
	for(int x = 0;x < 4;x++)
	{
		double *curInputs = inputs + x * nn.numInputs;
		double *curOutput = output + x * nn.numOutputs;
		actvHidden(nn, curInputs);
		actvOutput(nn);
		printf("Inputs: %lf and %lf got %lf expected %f \n", curInputs[0],curInputs[1],getMVal(nn.outputLayer,0,0),curOutput[x]);
	}
}



int main()
{

	srand(time(NULL));
	neuralNetwork xor = initNN(2,5,1);
	//train the network on 10k gens
	train(xor, trIn, trOut, 4, 10000, 4);
	//test the network for the 4possibiblite:s
	test(xor,trIn,trOut);


	// Save and load the network from a file
	neuralNetToFile(xor, "test.json");
	printf("%c",Output(xor));
//	neuralNetwork JSON = fileToNeuralNet("test.json");

//	printf("Test with the loaded wieghts and bias \n");
//	test(JSON, trIn, trOut);

	

	return 0;
}
