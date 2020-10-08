#include <stdio.h>
#include "nn.h"
#include <time.h>
#include <stdlib.h>

//debug part

int main()
{
	srand(time(NULL));
	neuralNetwork test = initNN(2,2,1);

	printf("BIAS = %lf",getMatVal(test.outputLayerBias,0,0));

	return 0;
}
