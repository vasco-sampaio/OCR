# include "neunet.h"
# include "load_set.h"
# include "neuralIO.h"


# include <stdlib.h>
# include <stdio.h>
# include <time.h>



void testNeuralNet(neunet_t *nn,double *inputs,double *outputs)
{
	int size = NN_OUTPUTS;
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
		char nn_answer = neural_net_ask(nn, curInputs);
		printf("Outputs: %c and expected %c || ", nn_answer,ExpOut(size,outputs+i*size));
	}
	printf("\n");
}

int main()
{
	srand(time(NULL));


	int set_size = 434;
	//train data_set in and out
	double *in = calloc(set_size * IMG_SIZE, sizeof(double));
	double *out = calloc(set_size * NN_OUTPUTS, sizeof(double));
	//test data_set in and out
	double *t_in = calloc(62 * IMG_SIZE,sizeof(double));
	double *t_out = calloc(62 * NN_OUTPUTS,sizeof(double));
	
	//load training set
	set_size = (int) load_dataset("../data_sets/arial_25_train", (size_t) set_size, in, out);
	printf("Loaded %i training images\n", set_size);

	//load test_set
	int test_size = (int) load_dataset("../data_sets/arial_25_test", (size_t) 62,t_in,t_out);
	printf("Loaded %i testing images\n", test_size);
	
	neunet_t *xou = init_neunet();

	for(int gen = 0; gen < 50; ++gen)
	{
		if(!(gen % 100))
			printf("Gen %i\n", gen);

		for(int el = 0; el < set_size; ++el)
		{
			double *curIn = in + el * NN_INPUTS;
			double *curOut = out + el * NN_OUTPUTS ;
			neunet_train(xou, curIn, curOut, 0.1);

		}
	}

	//testNeuralNet(xou,aNum,t_in,t_out);
	neuralNetToFile(xou, "test.nn");


	free(xou);
	neunet_t *bis = fileToNeuralNet("test.nn");
	testNeuralNet(bis,t_in,t_out);



	free(bis);

	//to remove warnings
	return test_size;
}
