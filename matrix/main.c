#include <stdio.h>
#include "nn.h"
#include <time.h>
#include <stdlib.h>

//debug part

int main()
{
	srand(time(NULL));
	neuralNetwork test = initNN(2,2,1);

	for(int i = 0;i<test.numHiddens;i++)
	{
		printf("Bias: %lf \n",getMatrixVal(test.hiddenLayerBias,i,0));
	}

	return 0;
}
