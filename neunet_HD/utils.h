# ifndef _UTILS_H
# define _UTILS_H

double sigmoid(double x);
double dSigmoid(double x);

// Return a random value in 0-1
double uniform(); 

double xavier_init(int n);


double dot_product(double *a, double *b, int len);

void apply_fct(double *a, double (*fvt)(double), int n);


void swap(int *a, int *b);

void shuffledList(int size, int *dst);

# endif //_UTILS_H
