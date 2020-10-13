#ifndef LAYER_H
#define LAYER_H
#include <stdlib.h>
#include "neuron.h"
#include <math.h>

typedef struct layer_t
{
	int nbNeu;
	neuron *neu;
}layer;

//Maths
float sigmoid(float x);
float dSigmoid(float x);
//end Maths

layer createLayer(int nbNeuron);
void initLayer(layer eLayer,int nbNodePr); //elayer = emptyLayer
void calcActv(layer Layer, layer previousLayer); //caclulate the activation of all the neuron in a
void printLayer(layer lay);
#endif
