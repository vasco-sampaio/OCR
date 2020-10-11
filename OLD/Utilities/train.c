# include "utility.h"
# include "forwardProp.h"

void trainNetwork(neunet_t *nn, double *inputs, double *expOutputs, double lr, int setSize, int epochs)
{
	for(int e = 0; e < epochs; e++)
	{
		int *indexes = shuffledList(setSize);
		for(int j = 0; j < setSize; j++)
		{
			matrix_t curInputs = matrixFromArray(inputs + (indexes[j]) * nn->nbInputs, 1, nn->nbInputs);
			matrix_t curOutputs = matrixFromArray(expOutputs + (indexes[j]) * nn->nbOutputs, 1, nn->nbOutputs);

			nn->outputAct = forwardProp(nn, curInputs);

			backProp(nn, curInputs, curOutputs, lr);

			if(e == epochs -1)
			{
				printf("Inputs:\n%f\n%f\nExpected: %f\nGot : %f\n",curInputs.data[0], curInputs.data[1], curOutputs.data[0], nn->outputAct.data[0]);
			}
		}
	}
}
