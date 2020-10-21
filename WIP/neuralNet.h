# ifndef _NEURALNET_H_
# define _NEURALNET_H_




// The weights are linking the layer to the previous one

typedef struct layer_t
{
	int size;
	int prevSize;
	double *biases;

	double *preAct;
	double *act;

	double *weights;

}nn_layer;


typedef struct neuralnet_t
{
	int nbInputs;
	int nbOutputs;
	int nbHiddenLayers;

	nn_layer output_layer;

	nn_layer *hidden_layers;

}neuralNet;


neuralNet initNeuralNet(int nbIn, int nbOut, int nbHidden, int *hiddenSizes);


void freeNeuralNet(neuralNet nn);

# endif // _NEURALNET_H_
