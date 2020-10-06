#ifndef TRAIN_H
#define TRAIN_H

float* deltaOutput(layer output, float expected);
float* deltaHiddens(layer hidden,float* deltaO, layer output);
void updateNeuron(layer inputs, layer output, layer hiddens, float* deltaH, float* deltaO, float lr, int nbInput);
void train(neuralNetwork nn);
void testNetwork(neuralNetwork nn,float x,float y,float expected);

#endif
