# ifndef _BACKPROP_H_
# define _BACKPROP_H_

double *backpropOutput(double *layer, int layerSize, double *expectedOutput);
double *backpropLayer(double *layer, double *nextWeights, double *nextLayerError, int layerSize, int nextLayerSize);

# endif // _BACKPROP_H_
