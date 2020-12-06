# include "matrix.h"
# include "utility.h"

# include <math.h>
# include <stdlib.h>
# include <time.h>
# include <err.h>
# include <stdio.h>



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



// Get the value of matrix at coordinates
double getMVal(matrix_t m, int row, int col)
{
	return m.data[row * m.cols + col];
}



// Set the value of coordinates row col of matrix at value
void setMVal(matrix_t m, int row, int col, double val)
{
	m.data[row * m.cols + col] = val;
}



// Add the value to the existing one
void addMVal(matrix_t m, int row, int col, double val)
{
	m.data[row * m.cols + col] += val;
}



// Print the given matrix
void printMatrix(matrix_t m)
{
	for(int i = 0; i < m.rows; i++)
	{
		for(int j = 0; j < m.cols; j++)
		{
			printf("%f ", getMVal(m, i, j));
		}
		printf("\n");
	}
	printf("\n");
}

