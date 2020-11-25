#include "nn.h"
#include "train.h"
#include "neuralIO.h"
#include "load_set.h"
#include "pixel_functions.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//debug part

double traininginputs[8] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f };
double trainingoutputs[4] = { 0.0f,1.0f,1.0f,0.0f };
double *trIn = traininginputs;
double *trOut = trainingoutputs;
char alphaNum[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char *aNum = alphaNum;

/*
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
		printf("Inputs: %lf and %lf got %lf expected %f \n", curInputs[0],curInputs[1],getMVal(nn.outputLayer,0,0),curOutput[0]);
	}
}
*/


int main()
{
	/*XOR PART  */
	srand(time(NULL));
	/*neuralNetwork xor = initNN(2,5,1);
	//train the network on 10k gens
	train(xor, trIn, trOut, 4, 10000, 4);
	//test the network for the 4possibiblite:s
	//test(xor,trIn,trOut);


	// Save and load the network from a file
	neuralNetToFile(xor, "test.json");
	printf("%c",Output(xor,aNum));
//	neuralNetwork JSON = fileToNeuralNet("test.json");

//	printf("Test with the loaded wieghts and bias \n");
//	test(JSON, trIn, trOut);*/


	/*Main NN PART */
	size_t set_size = 61;
	double *inputs = calloc(set_size * IMG_SIZE,sizeof(double));
	double *outputs = calloc(set_size * NN_OUT_SIZE,sizeof(double));
	//directory of the data set
	char *path = "../data_sets/arial";
	//load training set
	load_dataset(path,set_size,inputs,outputs);
	/* Training */
	/*for(size_t x = 0; x < set_size;x++)
	{
		print_output(outputs+NN_OUT_SIZE*x);
		printf("\n");
	}*/

	// Test Part for output -> OK 
	/*neuralNetwork test = initNN(IMG_SIZE,16,NN_OUT_SIZE);
	for(size_t i = 0;i < 62;i++)
	{
		setMVal(test.outputLayer,0,i,0);
	}
	setMVal(test.outputLayer,0,32,1);
	printf("Output: %c\n",Output(test,aNum));*/	

	/* Test part for expected output -> OK */
	/*double test[62];
	double ptest = *test;
	for(size_t i = 0;i < 62;i++)
	{
		test[i] = 0;
	}
	test[61] = 1;
	printf("ExpOutput: %c\n",ExpOut(62,test,aNum));*/
	neuralNetwork nn = initNN(IMG_SIZE,16,NN_OUT_SIZE);
	train(nn,inputs,outputs,61,100000,0.6,aNum);
	return 0;
}
