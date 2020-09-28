#ifndef LAYER_H
#define LAYER_H
#include <stdlib.h>
#include "neuron.h"

typedef struct layer_t
{
	int nbNeu;
	neuron *neu;
}layer;

layer createLayer(int nbNeuron);
void initLayer(layer eLayer,int nbNodePr); //elayer = emptyLayer


#endif
