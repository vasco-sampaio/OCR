# ifndef _UTILS_H_
# define _UTILS_H_

double randVal(double min, double max);

double sigmoid(double x);
double dSigmoid(double sig);

double ReLU(double x);

void softMax(double *array, int len);

void applyFunc(double *array, int len, double (*fct)(double));

void resetAlloc(double *ptr, int size);

# endif // _UTILS_H_
