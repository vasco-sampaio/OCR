# ifndef _NEURAL_TRAINING_H_
# define _NEURAL_TRAINING_H_


typedef struct trainData_t
{

	double *trainInputs;
	double *trainOutputs;
}trainData;

trainData trainingData(double *inputs, double *outputs);

void trainNetwork(neuralNet nn, trainData TD, int batchsize, double lr);


# endif	// _NEURAL_NETWORK_H_
