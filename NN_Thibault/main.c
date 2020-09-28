/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/09/2020 18:56:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "neuron.c"
#include "layer.c"
#include "nn.c"

int main()
{
	neuron neu = createNeuron(50);
	printf("This neuron as %f activation %ld weights and %f bias \n",neu.activation,sizeof(neu.weights),neu.bias);

	layer lay = createLayer(10);
	printf("Successfully create a layer with %i neuron", lay.nbNeu);
	
	neuralNetwork nn = initNN(2,1,1);
	initLayer(lay,2);
	
	for(int i = 0; i< lay.nbNeu;i++)
	{
		neu = lay.neu[i];
		printf("Neuron %i: bias: %f actv: %f \n",i,neu.bias, neu.activation);
	 	for(int j = 0; j < 2;j++)
	
		printf("weights %i: %f\n",j,neu.weights[j]);
	}
	
	return 0;
}
