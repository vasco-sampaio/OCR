# ifndef _NEURALIO_H_
# define _NEURALIO_H_

#include "neunet.h"

void neuralNetToFile(neunet_t *nn, char *path);
neunet_t *fileToNeuralNet(char *path);

# endif //_NEURALIO_H_
