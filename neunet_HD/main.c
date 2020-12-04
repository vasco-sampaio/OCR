# include "neunet.h"
# include "load_set.h"


# include <stdlib.h>
# include <stdio.h>
# include <time.h>



char alphaNum[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

char *aNum = alphaNum;


void testNeuralNet(neunet_t *nn,char *aNum,double *inputs,double *outputs)
{
	int size = nn->nb_outputs;
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
		for(int i = 0; i < nn->nb_inputs;i++)
		{
			nn->inputs[i] = curInputs[i];
		}
		forward_prop(nn);
		printf("Outputs: %c and expected %c || ",Output(nn,aNum),ExpOut(size,outputs+i*size,aNum));
	}
	printf("\n");
}

int main()
{
	srand(time(NULL));
	printf("%li\n", sizeof(neunet_t));
/*
	double d[] = {0.0,0.0,0.0,1.0,1.0,0.0,1.0,1.0};
	double o[] = {0.0,1.0,1.0,0.0};

	double *in = d;
	double *out = o;
*/

	int set_size = 434;
	double *in = calloc(set_size * IMG_SIZE, sizeof(double));
	double *out = calloc(set_size * OUTPUTS, sizeof(double));

	set_size = (int) load_dataset("../data_sets/arial_25_train", (size_t) set_size, in, out);

	printf("Loaded %i items\n", set_size);



	neunet_t *xou = init_neunet();

	for(int gen = 0; gen < 1000; ++gen)
	{
		

		for(int el = 0; el < set_size; ++el)
		{
			double *curIn = in + el * INPUTS;
			double *curOut = out + el * OUTPUTS;
			neunet_train(xou, curIn, curOut, 0.5);
			neunet_get_error(xou);

		}
		if(!(gen % 100))
		{
			printf("Gen %i\n", gen);
			printf("Error: %f\n", xou->gen_error);
		}
	}
	testNeuralNet(xou,aNum,in,out);
	free(xou);


}
