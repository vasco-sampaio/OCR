# ifndef _MATRIX_H_
# define _MATRIX_H_

double uniform();


// Matrix type definition
typedef struct
{
int cols, rows;
double *data;
} matrix_t;


matrix_t initMatrix(int rows, int cols);
matrix_t initRandMatrix(int rows, int cols);

void setMVal(matrix_t m, int row, int col, double value);
double getMVal(matrix_t m, int row, int col);

void printMatrix(matrix_t m);
# endif
