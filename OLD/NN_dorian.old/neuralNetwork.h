# ifndef _NEURALNETWORK_H_
# define _NEURALNETWORK_H_

void initNeuralNetwork(int inputSize, int hiddenSize, int outputSize);

void trainNetwork(int nbSets, double *inputs, double *outputs);

void computeNetwork(double *inputs);

double *outputAct;

void printNetwork();

# endif //_NEURALNETWORK_H_
