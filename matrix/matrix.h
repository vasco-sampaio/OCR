# ifndef _MATRIX_H_
# define _MATRIX_H_

typedef struct 
{
int width;
int height;
double *data;
} matrix_t;


matrix_t createMatrix(int width, int height);
void initValues(matrix_t m);
matrix_t initMatrix(int width, int heights);
double getMatrixVal(matrix_t m, int w, int h);
void setMatrixVal(matrix_t m, int w, int h, double value);
matrix_t matrixProduct(matrix_t a, matrix_t b);

void printMatrix(matrix_t m);

# endif //_MATRIX_H_
