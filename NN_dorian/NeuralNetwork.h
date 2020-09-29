# ifndef __NEURALNETWORK_H__
# define __NEURALNETWORK_H__

void initNeuralNetwork(int inputSize, int HlSize, int  outputLSize);
void computeValue(double *inputs);

void train(int trainSetSize, double *inputs, double *outputs);

double *outputLayerNodes;


void printNetwork();

# endif //__NEURALNETWORK_H__
