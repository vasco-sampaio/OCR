# ifndef _BACKPROP_H_
# define _BACKPROP_H_

# include "neuralNet.h"

void deltaOut(neuralNet nn, double *exp_outputs, double *delta);
void deltaPrevLayer(nn_layer layer, double *delta, double *prevAct);

void updateLayer(nn_layer layer, double *delta, double *inputs, double lr);


# endif // _BACKPROP_H_
