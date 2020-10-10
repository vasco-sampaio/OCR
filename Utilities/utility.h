# ifndef _UTILITY_H_
# define _UTILITY_H_

double sigmoid(double x);
double dSigmoid(double x);

double uniform();

typedef struct matrix
{
int rows;
int cols;
double *data;
}matrix_t;

matrix_t initMatrix(int rows, int cols);
matrix_t initRandMatrix(int rows, int cols);
matrix_t matrixFromArray(double *array, int rows, int cols);

void setMatVal(matrix_t m, int row, int col, double val);
void addMatVal(matrix_t m, int row, int col, double val);
double getMatVal(matrix_t m, int row, int col);

matrix_t matAdd(matrix_t a, matrix_t b);
matrix_t matSub(matrix_t a, matrix_t b);
matrix_t matMul(matrix_t a, matrix_t b);
matrix_t matTrans(matrix_t m);
matrix_t matFunc(matrix_t m, double(*fct)(double));

void printMatrix(matrix_t m);
void freeMatrix(matrix_t m);

# endif //_UTILITY_H_
