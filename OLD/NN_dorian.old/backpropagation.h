# ifndef _BACKPROP_H_
# define _BACKPROP_H_

void updateLayer(double *layer, double *weights, double *biases, double *delta, int layerSize, int prevLayerSize, double lr);

double *outputLayerError(double *outputs, double *target, int layerSize);

double *hiddenLayerError(double *act, double *nextLayerError, double *weights, int layerSize, int nextLayerSize);

# endif // _BACKPROP_H_
