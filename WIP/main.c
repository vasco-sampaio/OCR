# include "neuralNet.h"
# include "forProp.h"
# include "neuralTraining.h"

# include <stdio.h>
# include <stdlib.h>
# include <time.h>

double trainIn[] = {0.0,0.0,1.0,0.0,0.0,1.0,1.0,1.0};
double trainOut[] = {0.0,1.0,1.0,0.0};
double *trIn = trainIn;
double *trOut = trainOut;


int main()
{
	srand(time(NULL));

	int sizes[] = {2, 1};
	int *size = sizes;
	neuralNet X = initNeuralNet(2, 2, size);

	trainData TD = trainingData(trIn, trOut);

	for(int i = 0; i < 1000; ++i)
	{
		trainNetwork(X, TD, 4, 1);
	}


/*	double h_bias[] = {3.147704, 8.195579};
	double *hbias = h_bias;

	double o_bias[] = {-5.695310};
	double *obias = o_bias;

	//double h_w[] = {-7.415554,-5.495108,-7.392635,-5.491603};
	double h_w[] = {-7.415554,-7.392635,-5.495108,-5.491603};
	double *hw = h_w; 

	double o_w[] = {-11.869423,11.823280};
	double *ow = o_w;

	X.layers[0].biases = hbias;
	X.layers[0].weights = hw;
	X.layers[1].biases = obias;
	X.layers[1].weights = ow;
*/
	double in[] = {1.0,1.0};
	double *inn = in;
	processNetwork(X, inn);
	printf("1 and 1: %f\n", *X.layers[1].act);
	inn[0] = 0.0;
	processNetwork(X, inn);
	printf("0 and 1: %f\n", *X.layers[1].act);
	inn[1] = 0.0;
	processNetwork(X, inn);
	printf("0 and 0: %f\n", *X.layers[1].act);
	inn[0] = 1.0;
	processNetwork(X, inn);
	printf("1 and 0: %f\n", *X.layers[1].act);

	
	//freeNeuralNet(X);

	return 0;
}
