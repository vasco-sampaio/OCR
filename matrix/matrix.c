# include "matrix.h"

# include <stdlib.h>
# include <stdio.h>
# include <err.h>




// Initialize a matrix
matrix_t createMatrix(int width, int height)
{
	printf("Creting matrix of size %i, %i\n", width, height);
	matrix_t res;
	res.width = width;
	res.height = height;
	res.data = calloc(width * height, sizeof(double));

	return res;
}


// Set the value stored in m at point (i,j) to value
void setMatrixVal(matrix_t m, int i, int j, double value)
{
	m.data[i * m.width + j] = value;
}


// Return the value stored in matrix m at point (i,j)
double getMatrixVal(matrix_t m, int i, int j)
{
	return m.data[i * m.width + j];
}


// Return the product of matrices a and b
matrix_t matrixProduct(matrix_t a, matrix_t b)
{
	if (a.width != b.height)
	{
		errx(1, "Can't multiply those matrices, invalid dimensions");
	}

	matrix_t res = createMatrix(b.width, a.height);
	double val;
	for(int i = 0; i < a.height; i++)
	{
		for(int j = 0; j < b.width; j++)
		{
			val = 0;
			for(int k = 0; k < a.width; k++)
			{
				val += getMatrixVal(a, k, i) * getMatrixVal(b, j, k);
			}
			setMatrixVal(res, j, i, val);
		}
	}

	return res;
}

void printMatrix(matrix_t a)
{
	for(int i = 0; i < a.height; i++)
	{
		for(int j = 0; j < a.width; j++)
		{
			printf("%f | ", getMatrixVal(a, j, i));
		}
		printf("\n");
	}
}
