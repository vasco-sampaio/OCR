#ifndef TRAIN_H
#define TRAIN_H

double *deltaOutput(layer output, double expected);
double *deltaHiddens(layer hidden,double *deltaO, layer output);
void updateNeuron(layer inputs, layer output, layer hiddens, double *deltaH, double *deltaO, double lr, int nbInput);
void train(neuralNetwork nn);
void testNetwork(neuralNetwork nn,double x,double y,double expected);

#endif
