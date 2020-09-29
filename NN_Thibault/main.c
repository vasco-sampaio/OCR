/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main of the OCR projet
 *
 *        Version:  1.0
 *        Created:  27/09/2020 18:56:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Thibault Boutet, 
 *   Organization: None
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <time.h>
#include "neuron.c"
#include "layer.c"
#include "nn.c"
#include "train.c"


int main()

{
	srand(time(NULL));
/*   
	   neuron neu = createNeuron(50);
	printf("This neuron as %f activation %ld weights and %f bias \n",neu.activation,sizeof(neu.weights),neu.bias);

	layer lay = createLayer(10);
	printf("Successfully create a layer with %i neuron", lay.nbNeu);
	
	neuralNetwork nn = initNN(700,200,36); //test for OCR network
	initLayer(lay,2);
	
	for(int i = 0; i< lay.nbNeu;i++)
	{
		neu = lay.neu[i];
		printf("Neuron %i: bias: %f actv: %f \n",i,neu.bias, neu.activation);
	 	for(int j = 0; j < 2;j++)
	
		printf("weights %i: %f\n",j,neu.weights[j]);
	}
*/	
/*   neuralNetwork test = initNN(2,2,1);
	printLayer(test.hiddensTab);
	test.inputsTab.neu[0].activation = 0;
	test.inputsTab.neu[1].activation = 1;
	printf("Calcul of activation:\n  ");
	calcActv(test.hiddensTab,test.inputsTab);
	printLayer(test.hiddensTab);
	printf("Output of the NN: ");
	calcActv(test.outputTab,test.hiddensTab);
	printLayer(test.outputTab);
*/

	neuralNetwork Xor = initNN(2,2,1);
	train(Xor);
	return 0;
}
