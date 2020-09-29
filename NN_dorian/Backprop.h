# ifndef _BACKPROP_H_
# define _BACKPROP_H_

double *backpropOutput(double *layer, int layerSize, double *expectedOutput);
double *backpropLayer(double *layer, double *nextWeights, double *nextLayerError, int layerSize, int nextLayerSize);

void applyBackprop(double *delta, double *inputs, double *weights, double *biases, int layerSize, int inputSize, double learningRate);

# endif // _BACKPROP_H_
