#include "nn.h"

int nbTraining = 4;

double training_inputs[4][2] = {{0.0f,0.0f} , {0.0f,1.0f} , {1.0f,0.0f}, {1.0f,1.0f}};

double trainingOutputs[4] = {0.0f,1.0f,1.0f,0.0f};

double *deltaOutput(layer output, double expected)
{
	double *deltaO;
	deltaO = calloc(output.nbNeu,sizeof(double));
	double error;
	for(int i = 0;i < output.nbNeu;i++)
	{
		error = expected - output.neu[i].activation;
		deltaO[i] = error * dSigmoid(output.neu[i].activation);
	}

	return deltaO;
}

double *deltaHiddens(layer hidden, double* deltaO,layer output)
{
	double *deltaH;
	deltaH = calloc(hidden.nbNeu,sizeof(double));
	for(int i = 0; i < hidden.nbNeu;i++)
	{
		double error = 0.0f;
		for(int j = 0;j < output.nbNeu;j++)
		{
			error+=deltaO[j]*output.neu[0].weights[i];
		}
		deltaH[i] = error * dSigmoid(hidden.neu[i].activation);
	}

	return deltaH;
}


void updateNeuron(layer inputs,layer output,layer hiddens, double* deltaH, double* deltaO,double lr,int nbInput )//lr is the learning rate
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
	double *inputs = calloc(2,sizeof(double));
	double lr = 0.1f; //learning rate
	while(i <= 1000000)
	{
//		printf("Epoch n°%i \n ",i);
		int rd = (int) rand() % 4;
		inputs[0] = training_inputs[rd][0];
		inputs[1] = training_inputs[rd][1];
		double outputsExp = trainingOutputs[rd];
		nn.inputsTab.neu[0].activation = inputs[0];
		nn.inputsTab.neu[1].activation = inputs[1];
		//printLayer(nn.inputsTab);
		calcActv(nn.hiddensTab, nn.inputsTab);
		calcActv(nn.outputTab,nn.hiddensTab);
			
// 		printf("LR: %f\n",lr);
		double *deltaO = deltaOutput(nn.outputTab,outputsExp);
		double *deltaH = deltaHiddens(nn.hiddensTab,deltaO,nn.outputTab);
		updateNeuron(nn.inputsTab,nn.outputTab,nn.hiddensTab,deltaH,deltaO,lr,rd);
		printf("Value expected: %f, got %f error: %f \n ",outputsExp,nn.outputTab.neu[0].activation,fabs(outputsExp-nn.outputTab.neu[0].activation));
		i++;
	}
}
  
void testNetwork(neuralNetwork nn,double x, double y,double expected)
{

	nn.inputsTab.neu[0].activation = x;
	nn.inputsTab.neu[1].activation = y;
	calcActv(nn.hiddensTab,nn.inputsTab);
	calcActv(nn.outputTab,nn.hiddensTab);
	printf("Test with (%f ; %f) got %lf, but expect %f \n",x,y,nn.outputTab.neu[0].activation,expected);

}
