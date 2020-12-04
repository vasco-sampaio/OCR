# include "neunet.h"
# include "load_set.h"


# include <stdlib.h>
# include <stdio.h>
# include <time.h>







int main()
{
	srand(time(NULL));
	printf("%i\n", sizeof(neunet_t));
/*
	double d[] = {0.0,0.0,0.0,1.0,1.0,0.0,1.0,1.0};
	double o[] = {0.0,1.0,1.0,0.0};

	double *in = d;
	double *out = o;
*/

	int set_size = 62;
	double *in = calloc(set_size * IMG_SIZE, sizeof(double));
	double *out = calloc(set_size * OUTPUTS, sizeof(double));

	set_size = (int) load_dataset("../data_sets/arial_25_train", (size_t) set_size, in, out);




	neunet_t *xou = init_neunet();

	for(int gen = 0; gen < 1000; ++gen)
	{
		if(!(gen % 100))
			printf("Gen %i\n", gen);

		for(int el = 0; el < set_size; ++el)
		{
			double *curIn = in + el * INPUTS;
			double *curOut = out + el * OUTPUTS;
			neunet_train(xou, curIn, curOut, 0.5);

		}
	}



	free(xou);


}
