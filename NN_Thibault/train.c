 #include "nn.c"

int nbTraining = 4;

float training_inputs[4][2] = {{0.0f,0.0f} , {0.0f,1.0f} , {1.0f,0.0f}, {1.0f,1.0f}};

float trainingOutputs[4] = {0.0f,1.0f,1.0f,0.0f};

float* deltaOutput(layer output, float expected)
{
	float* deltaO;
	deltaO = calloc(output.nbNeu,sizeof(float));
	for(int i = 0;output.nbNeu;i++)
	{
		float error = expected - output[i].activation;
		deltaO[i] = error * dSigmoid(output[i].activation);
	}
	return delta0;
}

float* deltaHiddens(layer hidden, float* deltaO,layer output)
{
	float deltaH;
	deltaH = calloc(hidden.nbNeu,sizeof(float));
	for(int i = 0; i < hidden.nbNeu;i++)
	{
		float error = 0.0f;
		for(int j = 0;j < output.nbNeu;j++)
		{
			error+=deltaO[j]*output[j];
		}
		deltaH[i] = error * dSigmoid(hidden.neu[i].activation);
	}
}


void updateWeights(layer inputs,layer output,layer hiddens, float* deltaH, float* deltaO,float lr )//lr is the learning rate
{
	/*for(int i = 0;i<output.nbNeu;i++)
	{
		output[i].bias += delta0[i]*lr;
		for(int j = 0; j<hiddens.nbNeu;j++)
		{
			output[i].weights[j]+=hiddens[j].activation*deltaH[i]*lr;
		}
	}
	
	for(int i = 0;i<hiddens.nbNeu;i++)
	{
		hiddens[i].bias = deltaH[i] * lr;
		for(int j = 0; j < inputs.nbNeu;j++)
		{
			hiddens[i].weights[j] += 
		}
	}*/
}
