#include "nn.h"
#include "train.h"
#include "neuralIO.h"
#include "load_set.h"
#include "pixel_functions.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//debug part
char alphaNum[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char *aNum = alphaNum;

void testNeuralNet(neuralNetwork nn,char *aNum,double *inputs,double *outputs)
{
	int size = nn.numOutputs;
	int j = 0;
	for(int i = 0; i < size; i++)
	{
		if(j == 6)
		{
			printf("\n\n");
			j = 0;
		}
		j++;
		double *curInputs = inputs + i * IMG_SIZE;
		actvHidden(nn,curInputs);
		actvOutput(nn);
		printf("Outputs: %c and expected %c || ",Output(nn,aNum),ExpOut(size,outputs+i*size,aNum));
	}
	printf("\n");
}


int main()
{
	srand(time(NULL));
	
	/*Main NN PART */
	size_t set_size = 62;
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
	train(nn,inputs,outputs,set_size,5,0.6);
	testNeuralNet(nn,aNum,inputs,outputs);
	return 0;
}
