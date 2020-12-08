# include "neunet.h"
# include "load_set.h"
# include "neuralIO.h"
# include "neural_main.h"


# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <unistd.h>


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
		double *curInputs = inputs + i * NN_INPUTS;
		char nn_answer = neural_net_ask(nn, curInputs);
		printf("Outputs: %c and expected %c || ", nn_answer,expected_output(size,outputs+i*size));
	}
	printf("\n");
}



// Function to call to train a neural network
void neural_net_run_training(char *nn_path, char *dataset_path, int set_size, int gens)
{
	// Neural net loading or creation
	neunet_t *nn;
	if(access(nn_path, F_OK) == 0)
		nn = fileToNeuralNet(nn_path);
	else
		nn = init_neunet();

	// Loading of the dataset
	double *inputs = calloc(set_size * NN_INPUTS, sizeof(double));
	double *Xoutputs = calloc(set_size * NN_OUTPUTS, sizeof(double));
	set_size = (int) load_dataset(dataset_path, set_size, inputs, Xoutputs);
	printf("Loaded %i training images from %s\n", set_size, dataset_path);
	
	double *curIn;
	double *curOut;

	// Performing training, generation after another
	for(int g = 0; g < gens; ++g)
	{
		for(int el = 0; el < set_size; ++el)
		{
			curIn = inputs + el * NN_INPUTS;
			curOut = Xoutputs + el * NN_OUTPUTS;
			neunet_train(nn, curIn, curOut, LEARNING_RATE);
		}
		if(!(g % 100))
			printf("Gen %i : neural net error: %f\n", g, 0.5);//neunet_get_error(nn)); 
	}

	// Saving of the neural network
	neuralNetToFile(nn, nn_path);
	// Free memory
	free(nn);
}

