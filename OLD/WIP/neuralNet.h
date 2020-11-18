# ifndef _NEURALNET_H_
# define _NEURALNET_H_




// The weights are linking the layer to the previous one

typedef struct layer_t
{
	int size;
	int prevSize;
	double *biases;

	double *act;

	double *weights;

}nn_layer;


typedef struct neuralnet_t
{
	int nbInputs;
	int nbOutputs;
	int nbLayers;

	nn_layer *layers;

}neuralNet;


neuralNet initNeuralNet(int nbIn, int nbLayers, int *layerSizes);


void freeNeuralNet(neuralNet nn);

# endif // _NEURALNET_H_
