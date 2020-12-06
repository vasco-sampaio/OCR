#ifndef TRAIN_H
#define TRAIN_H
#include "nn.h"
#include <stdlib.h>

double* deltaOut(neuralNetwork,int x);
void train(neuralNetwork nn);
#endif
