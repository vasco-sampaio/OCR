# include "utility.h"

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <err.h>

// Compute the sigmoid function and its derivative
double sigmoid(double x) { return 1.0/(1.0+ exp(-x)); }
double dSigmoid(double x) { return x * (1.0 - x); }

static int isRandInit = 0;
// Return a random value in 0-1
double uniform()
{
	if(!isRandInit)
	{
		srand(time(NULL));
		isRandInit = 1;
	}
	return (double) rand() / (double) RAND_MAX;
}

// ABOUT MATRICES:
// ROWS WILL ALWAYS BE FIRST PARAMETER IN FUNCTIONS, 
// 
// As matrix_t store values in a single array,
// rows will be concatenated:
//	
//	[[ A, B, C],
//	 [ D, E, F]]  (2 rows, 3 cols) 
//
//	 will be stored as : 
//
//	[A, B, C, D, E, F]
//

// Return a matrix of asked size
matrix_t initMatrix(int rows, int cols)
{
	matrix_t res;
	res.cols = cols;
	res.rows = rows;
	res.data = calloc(rows * cols, sizeof(double));

	return res;
}


// Return a matrix with randomly initialized values
matrix_t initRandMatrix(int rows, int cols)
{
	matrix_t res = initMatrix(rows, cols);

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			res.data[i * cols + j] = uniform();
		}
	}

	return res;
}


matrix_t matrixFromArray(double *array, int rows, int cols)
{
	matrix_t res = initMatrix(rows, cols);
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			setMatVal(res, i, j, array[i * cols + j]);
		}
	}

	return res;
}



// Set the value of coordinates row col of matrix at value
void setMatVal(matrix_t m, int row, int col, double val)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"setMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, m.rows, m.cols);
	}

	m.data[row * m.cols + col] = val;
}


// Add the value to the existing one
void addMatVal(matrix_t m, int row, int col, double val)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"addMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, m.rows, m.cols);
	}

	m.data[row * m.cols + col] += val;
}


// Get the value of matrix at coordinates
double getMatVal(matrix_t m, int row, int col)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"getMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, m.rows, m.cols);
	}

	return m.data[row * m.cols + col];
}


// Return the addition of 2 matrices (not in place)
matrix_t matAdd(matrix_t a, matrix_t b)
{
	if(a.rows != b.rows || a.cols != b.cols)
	{
		errx(1, "matAdd: matrices aren't same size");
	}
	
	matrix_t res = initMatrix(a.rows, a.cols);

	for(int i = 0; i < res.rows; i++)
	{
		for(int j = 0; j < res.cols; j++)
		{
			setMatVal(res, i, j, getMatVal(a, i ,j) + getMatVal(b, i, j));
		}
	}

	return res;
}


// Return the matrice A-B
matrix_t matSub(matrix_t a, matrix_t b)
{
	if(a.rows != b.rows || a.cols != b.cols)
	{
		errx(1, "matSub: matrices aren't same size");
	}
	
	matrix_t res = initMatrix(a.rows, a.cols);

	for(int i = 0; i < res.rows; i++)
	{
		for(int j = 0; j < res.cols; j++)
		{
			setMatVal(res, i, j, getMatVal(a, i ,j) - getMatVal(b, i, j));
		}
	}

	return res;
}


// Return the result of a matrix product (not in place)
matrix_t matMul(matrix_t a, matrix_t b)
{
	if(a.cols != b.rows)
	{
		errx(1, "matMul: Matrices size Error : A:(%i,%i), B:(%i,%i)", a.rows, a.cols, b.rows, b.cols);
	}

	matrix_t res = initMatrix(a.rows, b.cols);

	for(int i = 0; i < a.rows; i++)
	{
		for(int j = 0; j < b.cols; j++)
		{
			for(int k = 0; k < a.cols; k++)
			{
				addMatVal(res, i, j, getMatVal(a, i, k) * getMatVal(b, k, j));
			}
		}
	}

	return res;
}


// Return the transposed matrix of m
matrix_t matTrans(matrix_t m)
{
	matrix_t trans = initMatrix(m.cols, m.rows);
	
	for(int i = 0; i < m.rows; i++)
	{
		for(int j = 0; j < m.cols; j++)
		{
			setMatVal(trans, j, i, getMatVal(m, i, j));
		}
	}
	
	return trans;
}


// Apply fct to all the values of m
matrix_t matFunc(matrix_t m, double(*fct)(double))
{
	matrix_t res = initMatrix(m.rows, m.cols);
	double x;
	for(int i = 0; i < m.rows; i++)
	{
		for(int j = 0; j < m.cols; j++)
		{
			x = getMatVal(m, i, j);
			//printf("TEST = %f\n", x);
			x = fct(x);
			//printf("CALL = %f\n", x);
			setMatVal(res, i, j, fct(getMatVal(m, i, j)));
		}
	}
	
	return res;
}


// Return the sum of the matrice
double matSum(matrix_t m)
{
	double res = 0;
	for(int i = 0; i < m.rows; i++)
	{
		for(int j = 0; j < m.cols; j++)
		{
			res += getMatVal(m, i, j);
		}
	}
	return res;
}


// Print the given matrix
void printMatrix(matrix_t m)
{
	for(int i = 0; i < m.rows; i++)
	{
		for(int j = 0; j < m.cols; j++)
		{
			printf("%f ", getMatVal(m, i, j));
		}
		printf("\n");
	}
	printf("\n");
}


// Free the memory allocated to a matrice
void freeMatrix(matrix_t m)
{
	free(m.data);
}
