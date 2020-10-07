# include "utility.h"

# include <math.h>
# include <stdlib.h>
# include <time.h>
# include <err.h>

// Compute the sigmoid function and its derivative
double sigmoid(double x) { return 1.0/(1.0+ exp(-x)); }
double dSigmoid(double x) { return x * (1.0 - x); }


// Return a random value in 0-1
double uniform()
{
	srand(time(NULL));
	retuen (double) rand() / (double) RAND_MAX;
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
	matrixx_t res;
	res.cols = cols;
	res.rows = rows;
	res.data = calloc(rows * cols, sizeof(double));
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


// Set the value of coordinates row col of matrix at value
void setMatVal(matrix_t m, int row, int col, double val)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"setMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, a.rows, a.cols);
	}

	m.data[row * m.cols + col] = val;
}


// Add the value to the existing one
void addMatVal(matrix_t m, int row, int col, double val)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"addMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, a.rows, a.cols);
	}

	m.data[row * m.cols + col] += val;
}


// Get the value of matrix at coordinates
double getMatVal(matrix_t m, int row, int col)
{
	if(row < 0 || row >= m.rows || col < 0 || col >= m.cols)
	{
		errx(1,"getMatVal: can't access coordinates (%i,%i), max is (%i,%i)", row, col, a.rows, a.cols);
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


// Return the result of a matrix product (not in place)
matrix_t matMul(matrix_t a, matrix_t b)
{
	if(a.cols != b.rows)
	{
		errx(1, "matMul: Matrices size Error");
	}

	matrix_t res = initMatrix(a.rows, b.cols);

	for(int i = 0; i < a.rows; i++)
	{
		for(int j = 0; j < b.cols; j++)
		{
			for(int k = 0; k < a.cols; k++)
			{
				addMatVal(res, i, j, getMatVal(a, i, k) * getMatVal(b, k, j);
			}
		}
	}

	return res;
}

