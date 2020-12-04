# include "neunet.h"


# include <stdlib.h>
# include <stdio.h>
# include <time.h>







int main()
{
	srand(time(NULL));
	printf("%i\n", sizeof(neunet_t));
	
	double d[] = {0.0,0.0,0.0,1.0,1.0,0.0,1.0,1.0};
	double o[] = {0.0,1.0,1.0,0.0};

	double *in = d;
	double *out = o;

	neunet_t *xou = init_neunet();

	for(int i = 0; i < 100000; ++i)
	{
		for(int el = 0; el < 4; ++el)
		{
			double *curIn = in + el * INPUTS;
			double *curOut = out + el * OUTPUTS;
			neunet_train(xou, curIn, curOut, 0.5);
		
		if(i == 99999)
			printf("%f, %f -> %f\n", xou->inputs[0], xou->inputs[1], xou->act_o[0]);
		}
	}

	

	free(xou);


}
