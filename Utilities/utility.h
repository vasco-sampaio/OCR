# ifndef _UTILITY_H_
# define _UTILITY_H_

double sigmoid(double x);
double dSigmoid(double x);

double uniform();


// Matrix type definition
typedef struct
{
int cols, rows;
double *data;
} matrix_t;


matrix_t initMatrix(int rows, int cols);
matrix_t initRandMatrix(int rows, int cols);

void setMatVal(matrix_t m, int row, int col, double value);
void addMatVal(matrix_t m, int row, int col, double value);
double getMatVal(matrix_t m, int row, int col);

matrix_t matAdd(matrix_t a, matrix_t b);
matrix_t matMul(matrix_t a, matrix_t b);


# endif
