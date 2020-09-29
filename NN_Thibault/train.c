#include "nn.h"

int nbTraining = 4;

float training_inputs[4][2] = {{0.0f,0.0f} , {0.0f,1.0f} , {1.0f,0.0f}, {1.0f,1.0f}};

float trainingOutputs[4] = {0.0f,1.0f,1.0f,0.0f};

float* deltaOutput(layer output, float expected)
{
	float* deltaO;
	deltaO = calloc(output.nbNeu,sizeof(float));
	for(int i = 0;i < output.nbNeu;i++)
	{
		float error = expected - output.neu[i].activation;
		deltaO[i] = error * dSigmoid(output.neu[i].activation);
	}
	return deltaO;
}

float* deltaHiddens(layer hidden, float* deltaO,layer output)
{
	float *deltaH;
	deltaH = calloc(hidden.nbNeu,sizeof(float));
	for(int i = 0; i < hidden.nbNeu;i++)
	{
		float error = 0.0f;
		for(int j = 0;j < output.nbNeu;j++)
		{
			error+=deltaO[j]*output.neu[0].weights[j];
		}
		deltaH[i] = error * dSigmoid(hidden.neu[i].activation);
	}
	return deltaH;
}


void updateNeuron(layer inputs,layer output,layer hiddens, float* deltaH, float* deltaO,float lr,int nbInput )//lr is the learning rate
{
	for(int i = 0;i<output.nbNeu;i++)
	{
		output.neu[i].bias += deltaO[i]*lr;
		for(int j = 0; j<hiddens.nbNeu;j++)
		{
			output.neu[i].weights[j]+=hiddens.neu[j].activation*deltaH[i]*lr;
		}
	}
	
	for(int i = 0;i<hiddens.nbNeu;i++)
	{
		hiddens.neu[i].bias += deltaH[i] * lr;
		for(int j = 0; j < inputs.nbNeu;j++)
		{
			hiddens.neu[i].weights[j] += (training_inputs[nbInput][j])*deltaH[j]*lr;
		}
	}
}

void train(neuralNetwork nn)
{
	int i  = 0;
	float *inputs = calloc(2,sizeof(float));
	while(i <= 1000)
	{
		int rd = (int) rand() % 3;
		inputs[0] = training_inputs[rd][0];
		inputs[1] = training_inputs[rd][1];
		float outputsExp = trainingOutputs[rd];
		nn.inputsTab.neu[0].activation = inputs[0];
		nn.inputsTab.neu[1].activation = inputs[1];
		calcActv(nn.hiddensTab, nn.inputsTab);
		calcActv(nn.outputTab,nn.hiddensTab);

		float *deltaO = deltaOutput(nn.outputTab,outputsExp);
		float *deltaH = deltaHiddens(nn.hiddensTab,deltaO,nn.outputTab);
		updateNeuron(nn.inputsTab,nn.outputTab,nn.hiddensTab,deltaH,deltaO,0.1,rd);
		printf("Value expected: %f, got %f error: %f \n ",outputsExp,nn.outputTab.neu[0].activation,outputsExp-nn.outputTab.neu[0].activation);
		i++;
	}
}
