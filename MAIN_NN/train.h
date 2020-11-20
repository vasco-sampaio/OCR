#ifndef TRAIN_H
#define TRAIN_H
#include "nn.h"

#include <stdlib.h>

double* deltaOut(neuralNetwork, double *expOutput);
void train(neuralNetwork nn, double *inputs, double *expOutputs, int setSize, int epochs, double lr, char *aNum);

#endif
