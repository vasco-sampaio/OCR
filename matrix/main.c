#include <stdio.h>
#include "nn.h"
#include <time.h>
#include <stdlib.h>
#include "train.h"
//debug part

int main()
{
	srand(time(NULL));
	neuralNetwork test = initNN(2,2,1);
	printf("Bias: %f \n",getMatVal(test.hiddenWeights,0,1));
	train(test);
	return 0;
}
